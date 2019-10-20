#include "main.h"


char colors[MAXKEYS];
char depths[MAXKEYS];

unsigned long gqueue[MAXSIGS];
int q_head;
int q_tail;

signatures edges[MAXKEYS]; // for BACKWARDS: list of signers for each signee
signatures redges[MAXKEYS]; // for FORWARDS: list of signees for each signer

extern double delta_x, delta_y;

extern int moving;

extern key vertices[MAXKEYS];

extern struct sig sigs[MAXTSIGS];
extern unsigned int num_sigs;
extern unsigned long num_keys;


void set_key_loc_o(unsigned long pos, unsigned char comp, double val)
{
  // override check
  vertices[pos].p[comp] = val;
}
void set_key_loc(unsigned long pos, unsigned char comp, double val)
{
  // check first that the key isn't being moved by the user
  if (!(moving && hasm(pos, MU)))
    set_key_loc_o(pos, comp, val);
}
double get_key_loc(unsigned long pos, unsigned char comp) { return vertices[pos].p[comp]; }

void set_kx(unsigned long pos, double val) { set_key_loc(pos, X, val); }
void set_ky(unsigned long pos, double val) { set_key_loc(pos, Y, val); }
void set_kx_o(unsigned long pos, double val) { set_key_loc_o(pos, X, val); }
void set_ky_o(unsigned long pos, double val) { set_key_loc_o(pos, Y, val); }
double get_kx(unsigned long pos) { return get_key_loc(pos, X); }
double get_ky(unsigned long pos) { return get_key_loc(pos, Y); }

void set_key_accel(unsigned long pos, unsigned char comp, double val) { vertices[pos].a[comp] = val; }
double get_key_accel(unsigned long pos, unsigned char comp) { return vertices[pos].a[comp]; }

void set_kax(unsigned long pos, double val) { set_key_accel(pos, X, val); }
void set_kay(unsigned long pos, double val) { set_key_accel(pos, Y, val); }
double get_kax(unsigned long pos) { return get_key_accel(pos, X); }
double get_kay(unsigned long pos) { return get_key_accel(pos, Y); }


double update_ep(unsigned long sig, char update)
{
  // Updates signature potential energy or returns it.
  
  if (update)
  {
    double dx=(vertices[sigs[sig].f].p[X] - vertices[sigs[sig].t].p[X]);
    double dy=(vertices[sigs[sig].f].p[Y] - vertices[sigs[sig].t].p[Y]);
    double x = sqrt(dx*dx + dy*dy)-L0;
    sigs[sig].ep = 0.5 * K1 * x*x;
  }
  return sigs[sig].ep;
}


double update_eps(char update)
{
  // Updates potential energy for all keys and returns total.
  
  unsigned long i;
  double tot=0;

  for (i=0; i<num_sigs; i++)
    tot+=update_ep(i, update);

  return tot;
}


double update_key_eps(unsigned long pos, char update)
{
  // Updates potential energy for all signatures of a given key.
  
  double tot=0;
  struct signature* s=redges[pos].head;

  while (s)
  {
    tot += update_ep(s->s->id, update);
    s = s->next;
  }

  // TODO count signatures in both directions

  /* s=edges[pos].head;
  while (s)
  {
    tot += update_ep(s->s->id, update);
    s = s->next;
  }*/

  /* electrostatic potential energy (disabled for performance reasons)

  for (i=0; i<num_keys; i++)
  {
    dx = vertices[pos].a[X] - vertices[i].a[X];
    dy = vertices[pos].a[Y] - vertices[i].a[Y];
    if (dx && dy)
      tot+=E/sqrt(dx*dx + dy*dy);
  }*/

  return tot;
}


void key_alter_accel(unsigned long pos, struct signature* s, char o)
{
  // Update key acceleration to take a signature into account.
  
  double l,dx,dy,px,py;

  dx = (vertices[pos].p[X] - vertices[next(s->s, o)].p[X]);
  dy = (vertices[pos].p[Y] - vertices[next(s->s, o)].p[Y]);
  l = sqrt(dx*dx + dy*dy); // l is the distance between the keys
 
  // (px, py) is the equilibrum position of spring
  px = vertices[next(s->s, o)].p[X]+ dx*L0 / l;
  py = vertices[next(s->s, o)].p[Y]+ dy*L0 / l;

  // add this to the acceleration (applying Hooke's law)
  vertices[pos].a[X] += K2 * (px-vertices[pos].p[X]);
  vertices[pos].a[Y] += K2 * (py-vertices[pos].p[Y]);
}

void report_key_loc(unsigned long pos) { printf("Key %lu is at %lf %lf\n", pos, get_kx(pos), get_ky(pos)); }
void key_loc(unsigned long pos)
{
  // Display key name and position.
  name_key_p(pos); report_key_loc(pos);
}


void update_key_accel(unsigned long pos)
{
  // Update key acceleration.

  struct signature* s=redges[pos].head;

  // reset acceleration
  vertices[pos].a[X] = 0;
  vertices[pos].a[Y] = 0;

  while (s)
  {
    // take all signatures into account
    key_alter_accel(pos, s, FORWARDS);
    s = s->next;
  }

  //TODO bidirectional
  /*
  s = edges[pos].head;

  while (s)
  {
    key_alter_accel(pos, s, BACKWARDS);
    s = s->next;
  }*/

  /* electrostatic (disabled)
  for (i=0; i<num_keys; i++)
  {
    dx = vertices[pos].a[X] - vertices[i].a[X];
    dy = vertices[pos].a[Y] - vertices[i].a[Y];
    if (dx && dy)
    {
      set_kax(pos, get_kax(pos) + (E / (dx * dx)));
      set_kay(pos, get_kay(pos) + (E / (dy * dy)));
    }
  }*/
}


void move_key(unsigned long pos, double step)
{
  // Move key following its acceleration.
  // The algorithm tries to find a position on the ray which minimises potential energy.

  double ep1, ep2, x, y, delta;

  ep1 = update_key_eps(pos, 0);
  ep2 = ep1;
  delta = 1; // for the loop

  // we move along the ray with bigger and bigger steps, as long as potential energy decreases
  // when it starts increasing again, we revert back to the last position
  // TODO improve (dichotomy)

  while (delta > 0)
  {
    // we move, and update ep
    ep1 = ep2;
    x = get_kx(pos);
    y = get_ky(pos);

    set_kx(pos, x + step*get_kax(pos));
    set_ky(pos, y + step*get_kay(pos));

    ep2 = update_key_eps(pos, 1);
    delta = ep1 - ep2;
    step*=MULT_STEP;
  }

  // rollback
  set_kx(pos, x);
  set_ky(pos, y);
  ep1 = update_key_eps(pos, 1);
}


void move_one_pass(double step, unsigned long number)
{
  unsigned long s[MAXKEYS]; //use malloc
  unsigned long i;
  unsigned long size=num_keys;
  unsigned long buf,chosen;

  // initialise identity permutation
  for (i=0; i<size; i++)
    s[i]=i;

  for (i=0; i<(number>num_keys?num_keys:number); i++)
  {
    // mark a key, and put it at the end of the permutation
    // so that it will not get marked again
    chosen=(unsigned long) ((double) size * ((double) rand() * (double) rand())/((RAND_MAX + 1.0)*(RAND_MAX + 1.0)));
    buf = s[chosen];
    s[chosen] = s[size];
    s[size] = buf;
    {
      update_key_accel(buf);
      move_key(buf, step);
    }
    size--;
  }

}


void move_key_step(unsigned long i)
{
  move_key(i, STEP);
}


void moving_adjust(unsigned long pos)
{
  // isn't there a way to avoid globals?

  set_kx_o(pos, get_kx(pos) + delta_x);
  set_ky_o(pos, get_ky(pos) + delta_y);
}

