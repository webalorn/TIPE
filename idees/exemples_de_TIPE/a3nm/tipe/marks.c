#include "main.h"


extern key vertices[MAXKEYS];

extern struct sig sigs[MAXTSIGS];
extern unsigned int num_sigs;
extern unsigned long num_keys;

extern char colors[MAXKEYS];
extern char depths[MAXKEYS];

extern signatures edges[MAXKEYS]; // for BACKWARDS: list of signers for each signee
extern signatures redges[MAXKEYS]; // for FORWARDS: list of signees for each signer

extern int select_mode;

extern double get_kx(unsigned long pos);
extern double get_ky(unsigned long pos);
extern unsigned long get_pos_from_id(unsigned long id);


void setm(unsigned long pos, char val) { vertices[pos].mark = val; }
void delm(unsigned long pos, char val) { vertices[pos].mark &= ~val; }
void rstm(unsigned long pos) { delm(pos, MALL); }
void addm(unsigned long pos, char val) { vertices[pos].mark |= val; }
void notm(unsigned long pos, char val) { vertices[pos].mark ^= val; }
char getm(unsigned long pos) { return vertices[pos].mark; }
char hasm(unsigned long pos, char val) { return (getm(pos) & val) == val; }

char findm(char val)
{ 
  unsigned long i;

  for (i=0; i<num_keys; i++)
    if (hasm(i, val))
      return i;

  return num_keys;
}

char is_in_tld(unsigned long pos, char* tld)
{
  return (!strcasecmp(vertices[pos].name + (strlen(vertices[pos].name) - strlen(tld))*sizeof(char), tld));
}

unsigned long count_tld(char* tld)
{
  unsigned int i, tot=0;
  for (i=0;i<num_keys;i++)
    if (is_in_tld(i, tld))
      tot++;
  return tot;
}

void addm_all(char val) { do_all_c(&addm, val); }
void delm_all(char val) { do_all_c(&delm, val); }
void notm_all(char val) { do_all_c(&notm, val); }
void setm_all(char val) { do_all_c(&setm, val); }
void delm_selected(char val) { oprm_all(val, MU, SELECT_REMOVE); }
void addm_selected(char val) { oprm_all(val, MU, SELECT_UNION); }

void addm_rand(unsigned long num, char val)
{
  // Add mark val to num random keys.
  // TODO should use malloc
  
  unsigned long s[MAXKEYS], i, size=num_keys, buf,chosen;

  // initialise identity permutation
  for (i=0; i<size; i++)
    s[i]=i;

  for (i=0; i<num; i++)
  {
    // mark a key, and put it at the end of the permutation so that it will not get marked again
    chosen=(unsigned long) ((double) size * ((double) rand() * (double) rand())/((RAND_MAX + 1.0)*(RAND_MAX + 1.0)));
    buf = s[chosen]; s[chosen] = s[size]; s[size] = buf;
    addm(s[size], val);
    size--;
  }
}


void mark(unsigned long pos) { addm(pos, MFT); }
void fmark(unsigned long pos) { addm(pos, MF); }
void tmark(unsigned long pos) { addm(pos, MT); }
void unmark(unsigned long pos) { delm(pos, MFT); }
void mark_all() { addm_all(MFT); }
void unmark_all() { delm_all(MFT); }
void mark_rand(unsigned long num) { addm_rand(num, MFT); }
void fmark_rand(unsigned long num) { addm_rand(num, MF); }
void tmark_rand(unsigned long num) { addm_rand(num, MT); }

char is_fmark(unsigned long pos) { return hasm(pos, MF); }
char is_tmark(unsigned long pos) { return hasm(pos, MT); }
char is_mark(unsigned long pos) { return hasm(pos, MFT); }
char is_seen(unsigned long pos) { return hasm(pos, MS); }

unsigned long count_m(char val)
{
  // Count val marks.
  unsigned long i,tot=0;
  for(i=0;i<num_keys;i++)
    if (hasm(i, val))
      tot++;
  return tot;
}

unsigned long count_marks() { return count_m(MFT); }
unsigned long count_fmarks() { return count_m(MF); }
unsigned long count_tmarks() { return count_m(MT); }
int selection_empty() { return (count_m(MU) == 0); }

void addm_neighbours(unsigned long pos, char m, char direction)
{
  struct signature * s;

  if (direction == FORWARDS) s = redges[pos].head;
  else  s = edges[pos].head;

  while (s)
  {
    if (direction == FORWARDS) 
      addm(s->s->t, m);
    else 
      addm(s->s->f, m);
    s = s->next;
  }
}


void do_by_tld(void (*fun)(unsigned int, char), char* tld, char val)
{
  unsigned int i;
  for (i=0;i<num_keys;i++)
    if (is_in_tld(i, tld))
      (*fun)(i,val);
}

void add_by_tld(char* tld, char val) { do_by_tld(&addm, tld, val); }
void del_by_tld(char* tld, char val) { do_by_tld(&delm, tld, val); }

void mark_by_tld(char* tld) { add_by_tld(tld, MFT); }
void fmark_by_tld(char* tld) { add_by_tld(tld, MF); }
void tmark_by_tld(char* tld) { add_by_tld(tld, MT); }
void unmark_by_tld(char* tld) { del_by_tld(tld, MFT); }


void oprm(unsigned long i, char a, char b, char op)
{
  // Applies operator to marks a and b.
  
  switch(op)
  {
    case SELECT_SET:
      if (hasm(i,b)) addm(i,a); else delm(i,a);
      break;
    case SELECT_UNION:
      if (hasm(i,b)) addm(i,a);
      break;
    case SELECT_INTERSECTION:
      if (hasm(i,b) && hasm(i, a)) addm(i,a); else delm(i,a);
      break;
    case SELECT_REMOVE:
      if (hasm(i,b)) delm(i, a);
      break;
    case SELECT_XOR:
      if (((hasm(i, b)) || hasm(i, a)) && !(hasm(i, b) && hasm(i, a))) addm(i, a); else delm(i, a);
      break;
  }
}

void oprm_all(char a, char b, char op)
{
  unsigned long i;
  for (i=0; i<num_keys; i++)
    oprm(i, a, b, op);
}


void opsel_by_tld(char* tld)
{
  add_by_tld(tld, MV);
  printf("Marked keys ending in %s\n", tld);
  oprm_all(MU, MV,select_mode);
  delm_all(MV);
}

void opsel_by_id_p(unsigned long id_p)
{
  addm(get_pos_from_id(id_p), MV);
  printf("Marked key %lx \n", id_p);
  oprm_all(MU, MV,select_mode);
  delm_all(MV);
}

void opsel_by_id(char* id) { opsel_by_id_p(strtol(id, NULL, 16)); }

void opsel_by_region_p(double x1, double y1, double x2, double y2)
{
  unsigned long i;
  double kx, ky;

  for (i=0; i<num_keys; i++)
  {
    kx = get_kx(i); ky = get_ky(i);
    if (((x1 < kx && kx < x2) || ((x2 < kx) && (kx < x1)))
      && ((y1 < ky && ky < y2) || ((y2 < ky) && (ky < y1))))
      addm(i, MV);
  }

  oprm_all(MU, MV, select_mode);
  delm_all(MV);

  redraw_all(0);
}

void opsel_rand(char* num)
{
  unsigned long num_p;

  num_p = strtol(num, NULL, 10);
  addm_rand(num_p,MV);
  printf("Marked %lu random keys.\n", num_p);
  oprm_all(MU, MV,select_mode);
  delm_all(MV);
}


void addm_m_neighbours(char m1, char m2, enum graph_orientation o)
{
  // Add mark m2 to neighbours of keys with m1 (neighbour means signer or signee according to the orientation).

  unsigned long i;

  for (i=0; i<num_keys; i++)
    if (hasm(i, m1))
      addm_neighbours(i, m2, o);
}

