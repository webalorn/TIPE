#include "main.h"


extern key vertices[MAXKEYS];

extern struct sig sigs[MAXTSIGS];
extern unsigned int num_sigs;
extern unsigned long num_keys;

extern char colors[MAXKEYS];
extern char depths[MAXKEYS];

extern signatures edges[MAXKEYS];
extern signatures redges[MAXKEYS];

extern void circle(unsigned long pos, double radius, double cx, double cy);

int id(int a) { return a; }

char check(int argc, char** argv)
{
  // Check options syntax.

  char* base_name = 0;

  if ((base_name = strrchr(argv[0],'/')))
    base_name++;

  if(argc == 1)
  {
    printf("Usage: base_name WOTFILE\n");
    return ERR_SYNTAX;
  }

  return 0;
}

int create_temp_dir() { return system("mkdir -p temp"); }

int run_ar() { return system("cd temp; ar x wot_data"); }

enum error prepare(char* input)
{
  // Extract files from the Wotsap file.

  try(&create_temp_dir, &id, ERR_TEMPDIR, "Error: Cannot create temp folder.\n");

  char command[500];
  sprintf((char*) &command, "bunzip2 -cd %s > temp/wot_data", input);

  if (system(command))
  {
    fprintf(stderr,"Error: Cannot bunzip input file.\n");
    return ERR_BUNZIP;
  }

  try(&run_ar, &id, ERR_AR, "Error: ar did not succeed.\n");

  return ERR_NONE;
}


void read_sig(FILE *f, unsigned long signer, unsigned long sig_data)
{
  // Load a signature.

  struct signature *new_sig;

  new_sig = malloc(sizeof(struct signature));
  new_sig->next = NULL;
  new_sig->s = &sigs[num_sigs];
  push(&(edges[num_keys]), new_sig);
}

void redge_init(unsigned long i)
{
  redges[i].head = NULL; redges[i].last = NULL;
}

void reverse()
{
  // Create reverse adjacency lists (by reversing existing lists).

  unsigned long i;
  struct signature *s, *new_sig;

  do_all(&redge_init);
  for (i=0; i<num_keys; i++)
  {
    s = edges[i].head;

    while (s)
    {
      redges[s->s->f].num++;
      new_sig = malloc(sizeof(struct signature));
      new_sig->s = s->s;
      new_sig->next = NULL;
      push(&(redges[s->s->f]), new_sig);
      s = s->next;
    }
  }
}

enum error load(char* input)
{
  unsigned long i;

  FILE *f_debug, *f_keys, *f_names, *f_readme, *f_signatures, *f_wotversion;

  unsigned int sig_data;

  // TODO check for errors
  char ret=0;
  ret = prepare(input);
  if (ret) return ret;

  f_debug = fopen("temp/debug", "r");
  f_keys = fopen("temp/keys", "r");
  f_names = fopen("temp/names", "r");
  f_readme = fopen("temp/README", "r");
  f_signatures = fopen("temp/signatures", "r");
  f_wotversion = fopen("temp/WOTVERSION", "r");

  while (!feof(f_signatures) && !feof(f_names) && !feof(f_keys))
  {
    vertices[num_keys].id = 0;
    vertices[num_keys].id=read_ul(f_keys);
    fgets(vertices[num_keys].name, MAXNAMELEN, f_names);
    circle(num_keys, RAND_MAX/RAND_DIVIDE_FACTOR, RAND_MAX, RAND_MAX);
    vertices[num_keys].color = blue;
    trim_trailing_newline(num_keys);
    edges[num_keys].num=read_ul(f_signatures);
    edges[num_keys].head = NULL;
    edges[num_keys].last = NULL;
    rstm(num_keys);

    for (i=0; i<edges[num_keys].num; i++)
    {
      sig_data = read_ul(f_signatures);
      sigs[num_sigs].t = num_keys;
      sigs[num_sigs].f = get_id_from_sig_data(sig_data);
      sigs[num_sigs].type = get_type_from_sig_data(sig_data);
      sigs[num_sigs].ep = 0;
      sigs[num_sigs].id = num_sigs;
      read_sig(f_signatures, num_keys, sig_data);
      num_sigs++;
    }
    num_keys++;
  }

  // slight offset problem
  num_keys--;

  reverse();

  fclose(f_debug); 
  fclose(f_keys); 
  fclose(f_names); 
  fclose(f_readme); 
  fclose(f_signatures); 
  fclose(f_wotversion); 

  // TODO: cleanup temp dir
  return ERR_NONE;
}

