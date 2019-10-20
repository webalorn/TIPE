#include "main.h"


// array of keys
key vertices[MAXKEYS];
unsigned long num_keys = 0;

// array of signatures
struct sig sigs[MAXTSIGS];
unsigned int num_sigs=0;

// parameters for graphical interface
char auto_recalc=0;
char select_mode=SELECT_SET;
int per_pass=PER_PASS;


int main(int argc, char **argv)
{
  int rsl;
  char msg[500];

  // check command line parameters
  check(argc, argv);
  // initialise globals
  globals_init();
  // load data file
  load(argv[1]);
  // initialise graphics
  graphics_init();
  // initialise display
  reset_frame();

  sprintf(msg, "%lu keys loaded, %lu signatures.", num_keys, num_sigs);
  help(msg);

  while (1)
  {
    // redraw all keys
    redraw_all(update_eps(0));

    // improve key placement with force-directed algorithm
    if (auto_recalc)
      move_one_pass(STEP, per_pass);

    // manage user events
    while ((rsl = manage_event()) > 0);
    // exit if user requests it
    if (rsl<0) break;
  }

  // unload graphics
  graphics_end();

  return 0;
}

