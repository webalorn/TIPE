#include "main.h"


extern unsigned long num_keys;
extern key vertices[MAXKEYS];
extern signatures edges[MAXKEYS];
extern signatures redges[MAXKEYS];

extern double get_kx(unsigned long pos);
extern double get_ky(unsigned long pos);



void try(void* (*fun)(void), int (*predicate)(void*), enum error errcode, char* errdesc)
{
  // Try doing fun, and check its return value with predicate. If predicate returns TRUE, fun has failed: exit with errcode and message errdesc.
  
  if ((*predicate)((*fun)()))
  {
    fprintf(stderr, errdesc);
    exit(errcode);
  }
}

int is_null(void* a) { return a==NULL; }

void do_all(void (*fun)(unsigned long int))
{
  // Apply fun to every key.

  unsigned long i;

  for (i=0; i<num_keys; i++)
    (*fun)(i);
}

// different versions according to fun's parameters
// TODO: isn't there a better way?
void do_all_c(void (*fun)(unsigned long int, char), char c)
{
  unsigned long i;
  for (i=0; i<num_keys; i++)
    (*fun)(i, c);
}

void do_all_d(void (*fun)(unsigned long int, double), double c)
{
  unsigned long i;
  for (i=0; i<num_keys; i++)
    (*fun)(i, c);
}

void do_all_m(void (*fun)(unsigned long int), char m)
{
  // Apply to all keys with mark m.

  unsigned long i;

  for (i=0; i<num_keys; i++)
    if (hasm(i, m))
      (*fun)(i);
}

void do_all_selected(void (*fun)(unsigned long int)) { do_all_m(fun, MU); }


unsigned long get_pos_from_id(unsigned long id)
{
  // Get key index in vertices from its OpenPGP id.

  unsigned long i;

  for (i=0; i<num_keys; i++)
    if (vertices[i].id == id)
      return i;

  printf("ERROR: invalid key requested.\n");
  return MAXKEYS;
}

unsigned long read_ul(FILE *f)
{
  // Read an unsigned long from a file.

  unsigned long a=0;
  a |= (fgetc(f) << 24); if (feof(f)) return 0;
  a |= (fgetc(f) << 16); if (feof(f)) return 0;
  a |= (fgetc(f) << 8);  if (feof(f)) return 0;
  a |= (fgetc(f));
  return a; 
}

void trim_trailing_newline(unsigned long pos)
{
  int n = strlen(vertices[pos].name);
  if (vertices[pos].name[n - 1] == '\n')
    vertices[pos].name[n - 1] = '\0';
}

unsigned long get_type_from_sig_data(unsigned long sig_data)
{
  // Handles the signature data from the Wotsap file.
  return sig_data >> 28;
}
unsigned long get_id_from_sig_data(unsigned long sig_data)
{
  return (get_type_from_sig_data(sig_data) << 28) ^ sig_data;
}


void name_key_p(unsigned long pos)
{
  // Display key name, and show if it is marked or not.

  if (is_mark(pos)) printf("(*) "); else printf("    ");
  printf("%x - %s (%lu)\n", vertices[pos].id, vertices[pos].name, pos);
}

void report_on_key(unsigned long id)
{
  // Display some info about a key.

  unsigned long pos = get_pos_from_id(id);
  unsigned long n=edges[pos].num;
  struct signature * s = edges[pos].head;

  printf("Key %lu is %lu in WOT database.\n", id, pos);
  printf("It belongs to %s.\n", vertices[pos].name);
  printf("It has been signed by %lu keys:\n", n);

  while (s)
  {
    name_key_p(s->s->f); s = s->next;
  }

  printf("It has signed %lu keys:\n", n);

  s = redges[pos].head;
  while (s)
  {
    name_key_p(s->s->t); s = s->next;
  }

  breadth_explore(pos, FORWARDS, 1); breadth_explore(pos, BACKWARDS, 1);
}


char get_tld(unsigned long pos, char* tld, char mlen)
{
  // Get TLD of a key.

  int dot_pos=strlen(vertices[pos].name);
  int i,tld_len;
  if (vertices[pos].name[dot_pos-1] != '>')
    return 1; // no email address
  for(i=0;i<mlen;i++)
    if(vertices[pos].name[dot_pos-i] == '.')
      break;
  tld_len = i;
  dot_pos = dot_pos - tld_len;
  for (i=0;i<tld_len;i++)
    tld[i] = vertices[pos].name[dot_pos+i];
  tld[tld_len] = '\0';
  return 0;
}


unsigned long find_closest_m(double x, double y, int val, double dist)
{
  // Find closest key to a point with a specific mark (compute all distances and check which one is smaller)
  // dist is minimum distance for matching

  double dx, dy, d;
  unsigned long best=num_keys; // if nothing is found
  unsigned long i;

  for (i=0; i<num_keys; i++)
    if (hasm(i, val))
    {
      dx = get_kx(i) - x;
      dy = get_ky(i) - y;
      d = dx*dx + dy*dy;
      if (d < dist)
      {
        best = i; dist = d;
      }
    }

  return best;
}

unsigned long find_closest(double x, double y, double dist) { return find_closest_m(x, y, MNONE, dist); }

void strip_gt(char* tld)
{
  // Strip the '>' from "<foo@example.com>".

  int i=0;

  while (tld[i] != 0)
    i++;
  if (i>0 && tld[i-1] == '>')
    tld[i-1] = 0;
}


void circle(unsigned long pos, double radius, double cx, double cy)
{
  // Put key pos at a random position in a circle.
  
  double rnd;

  rnd = rand();

  vertices[pos].p[X] = cos(rnd) * radius + cx;
  vertices[pos].p[Y] = sin(rnd) * radius + cy;
}

