#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "SDL_Input/SDL_Input.h"
#include "SDL_Input/SDL_Input_TTF/SDL_Input_TTF.h"


// Limits. TODO: use malloc()!
#define MAXKEYS 50000
#define MAXNAMELEN 500
#define MAXSIGS 50000
#define MAXTSIGS 500000 // total number of sigs

// Fine-tuning.
// TODO: allow the user to change these at runtime.
#define PER_PASS 10000 // how many keys to update per pass
#define MIN_MOVED PER_PASS/20
#define K1 1 // spring constant for energy
#define K2 1 // spring constant for pull
#define L0 1000 // spring default length
#define F 0.005 // accel factor
#define STEP 0.001 // move step
#define MULT_STEP 1.01 // increase in move step (to avoid getting stuck on a key)
#define DISPRATE 100 // how often shall we output status info
#define TLD_THRESHOLD 10 // min keys in TLD to take it into account

// Marks are binary masks which can be applied to vertices. They are used as a naive way to keep track of key subsets or key options.
#define MNONE 0 // no mark
#define MF 1 // mark from: compute distances from these keys
#define MT 2 // mark to: compute distances to these keys
#define MFT 3 // mark from to
#define MS 4 // mark seen: the key or TLD has already been done
#define MU 8 // user mark (selection): the key is in user selection
#define MV 16 // instant user mark (result of last selection command)
#define MI 32 // display key id
#define MN 64 // display key name
#define MA 128 // display accel
#define MALL 255

enum operator { SELECT_SET, SELECT_UNION, SELECT_INTERSECTION, SELECT_REMOVE, SELECT_XOR };

enum error { ERR_NONE, ERR_SYNTAX, ERR_TEMPDIR, ERR_BUNZIP, ERR_AR, ERR_QOF, ERR_QUF, ERR_SDL_INIT, ERR_TTF_INIT, ERR_FONT, ERR_SDL_INPUT_INIT, ERR_VIDEO_INIT };

enum graph_color { BLACK, GRAY, WHITE }; // for BFS

enum coordinate { X, Y };

// FORWARDS is from signer to signee
// BACKWARDS is from signee to signer
enum graph_orientation { FORWARDS, BACKWARDS };

enum tooltip { TOOLTIP_NONE, TOOLTIP_HELP, TOOLTIP_INPUT };

// these are constants, but we don't initialise them right away
SDL_Color black;
SDL_Color white;
SDL_Color red;
SDL_Color green;
SDL_Color blue;

#define WIDTH 1280
#define HEIGHT 800
#define DEPTH 32

#define FONT "fonts/Tuffy.ttf"
#define INTERFACE_FONT_SIZE 18
#define KEY_FONT_SIZE 12

#define ZFACTOR 10.0 // zoom speed with mouse wheel

// key size on screen
#define DOT_SIZE 2

// selection rectangle size
#define SELECTION_RECT_SIZE 2

// margin around the keys when fitting view
#define FITOFFSET 0.1
// added to dimensions when fitting view
#define KEYSIZE 10000

// minimum distance for key selection (in pixels, more or less)
#define MIN_DIST 200

// difference in pixels to distinguish click and multi-select
#define SELECTION_DELTA 20

#define TOOLTIP_TIME 100

// adjust size of graph representation
#define RAND_DIVIDE_FACTOR 1000.0



typedef struct {
  // TODO weird, should be unsigned long
  unsigned int id;
  char name[MAXNAMELEN];
  char mark;
  double p[2]; // position on planar representation
  double a[2]; // acceleration on planar representation
  SDL_Color color;
} key;

struct signature {
  // a member of a chained list of signatures
  struct sig * s; //the real signature
  struct signature * next;
};

struct sig {
  // a signature
  unsigned long f; //from: signer
  unsigned long t; //to: signee
  char type;
  double ep; // potential energy for planar representation
  unsigned long id;
};

typedef struct {
  // a chained list of signatures
  unsigned long num; //useless
  struct signature * head;
  struct signature * last; //should be useless
} signatures;

typedef struct {
  unsigned long q[MAXSIGS];
  unsigned long h; // head pos
  unsigned long t; // tail pos
} queue;



int id(int a);

void update_key_accel(unsigned long pos);
void move_key_step(unsigned long i);

void opsel_by_color(char*);
void opsel_by_id(char*);
void opsel_by_tld(char*);
void opsel_rand(char*);

void moving_adjust(unsigned long pos);
void opsel_by_region_p(double x1, double y1, double x2, double y2);

double disp2real_x(double disp_x);
double disp2real_y(double disp_y);
double sqrt(double x);
