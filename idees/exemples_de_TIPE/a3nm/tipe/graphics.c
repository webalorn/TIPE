#include "main.h"


double ax, ay, bx, by; // frame

extern double delta_x, delta_y;
extern char inputting;

char tooltip=0;
char tooltip_time=0;
void *input_callback;

int width=WIDTH;
int height=HEIGHT;

// for rectangle selection
int mouse_x, mouse_y;
// to display rectangle on mouse move
char mouse_byrect=0;

// for mouse button drag and drop
int mouse_mx, mouse_my;
// matching real coordinates
double mouse_mx_r, mouse_my_r;
// to refresh on mouse move
char mouse_middle=0;

char status[500];

TTF_Font *font = NULL;
TTF_Font *small_font = NULL;

SDL_Surface *screen = NULL;
SDL_Input_TTF *ttf = NULL;
SDL_Rect inputpos;

extern int id(int a);
extern int is_null(void* a);

extern double get_kx(unsigned long pos);
extern double get_ky(unsigned long pos);
extern double get_kax(unsigned long pos);
extern double get_kay(unsigned long pos);
extern void circle(unsigned long pos, double radius, double cx, double cy);

extern SDL_Color had_color;
SDL_Rect pos_status;

extern key vertices[MAXKEYS];

extern struct sig sigs[MAXTSIGS];
extern unsigned int num_sigs;
extern unsigned long num_keys;

extern signatures edges[MAXKEYS]; // for BACKWARDS: list of signers for each signee
extern signatures redges[MAXKEYS]; // for FORWARDS: list of signees for each signer

extern int pointer_x, pointer_y;



int sdl_init() { return SDL_Init(SDL_INIT_VIDEO); }

// TODO segfaults when the window is resized
int sdl_setvideomode() { return (screen = SDL_SetVideoMode(width, height, DEPTH, SDL_HWSURFACE | SDL_RESIZABLE)); } 

int load_font() { return (font = TTF_OpenFont(FONT, INTERFACE_FONT_SIZE)) != NULL; }
int load_small_font() { return (small_font = TTF_OpenFont(FONT, KEY_FONT_SIZE)) != NULL; }

int load_input_field() { return (ttf = SDL_Input_TTF_Create( MAXNAMELEN, font, white, &black, inputpos, SDL_INPUT_TTF_SOLID )) != NULL; }


enum error graphics_init()
{
  // Run all initialisation functions, and fail if one of them fails.
  try(&sdl_init, &id, ERR_SDL_INIT, "Erreur d'initialisation de la SDL.\n");
  try(&TTF_Init, &id, ERR_TTF_INIT, "Erreur d'initialisation de la SDL_TTF.\n");
  try(&sdl_setvideomode, &is_null, ERR_VIDEO_INIT, "Erreur d'initialisation du mode graphique.\n");
  SDL_WM_SetCaption("Key visu", NULL);
  try(&load_font, &is_null, ERR_FONT, "Erreur de chargement de la police.\n");
  try(&load_small_font, &is_null, ERR_FONT, "Erreur de chargement de la police.\n");
  try(&load_input_field, &is_null, ERR_SDL_INPUT_INIT, "Erreur de chargement de la SDL_Input_TTF.\n");

  SDL_EnableKeyRepeat( SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL );

  SDL_EnableUNICODE( 1 );

  return ERR_NONE;
}


void globals_init()
{
  pos_status.x = 0;
  pos_status.y = 0;

  inputpos.x = 0;
  inputpos.y = 20;
  black.r = 0; black.g = 0; black.b = 0;
  white.r = 0xff; white.g = 0xff; white.b = 0xff;
  red.r = 0xff; red.g = 0; red.b = 0;
  green.r = 0; green.g = 0xff; green.b = 0;
  blue.r = 0; blue.g = 0; blue.b = 0xff;
  had_color.r = 0; had_color.g = 0; had_color.b = 0xff;
}


char valid_coord(int x, int y)
{
  // Check if coordinates are valid.
  return (x>=0) && (x < width) && (y>0) && (y < height);
}


void pixel_draw(int x, int y, Uint8 r, Uint8 g, Uint8 b, char size)
{
  // Draw a key.

  Uint32 *p;
  int i, j;

  for (i=-(size/2);i<(size/2+1);i++)
    for (j=-(size/2);j<(size/2+1);j++)
      if (valid_coord(x+i, y+j))
      {
        p = screen->pixels + (y+j) * screen->pitch + (x+i) * screen->format->BytesPerPixel;
        *p|=SDL_MapRGB(screen->format, r, g, b);
      }
}


void set_status(char * stat)
{
  if (!tooltip)
  {
    if (strcmp(status,stat))
      printf("%s\n", stat);
    strcpy(status, stat);
  }
}


void update_status()
{
  writeTxt(screen,status,0,0,white,font);
}


void remove_tooltip()
{
  tooltip=TOOLTIP_NONE;
  tooltip_time=0;
}


void set_tooltip(char t, char * ttip)
{
  remove_tooltip();
  set_status(ttip);
  tooltip = t;
  tooltip_time = TOOLTIP_TIME;
}


void update_tooltip()
{
  if (tooltip)
  {
    tooltip_time--;
    if (!tooltip_time)
      tooltip=TOOLTIP_NONE;
  }
}


void update_input()
{
  if (inputting)
    SDL_Input_TTF_Display( ttf, screen, NULL );
}


void fit_frame(unsigned long i)
{
  // Adjust frame to fit a key.

  if (get_kx(i) < ax || ax < 0)
    ax = get_kx(i) - KEYSIZE;
  if (get_ky(i) < ay || ay < 0)
    ay = get_ky(i) - KEYSIZE;
  if (get_kx(i) > bx)
    bx = get_kx(i) + KEYSIZE;
  if (get_ky(i) > by)
    by = get_ky(i) + KEYSIZE;
}


void reset_frame_m(int val) {
  // Adjust frame to fit keys with mark val.

  double dx, dy;
  double bax=ax, bbx=bx, bay=ay, bby=by;

  bx=by=0;
  ax=ay=-1;
  do_all_m(&fit_frame, val);

  dx = bx-ax;
  dy = by-ay;
  ax-=dx*FITOFFSET;
  bx+=dx*FITOFFSET;
  ay-=dy*FITOFFSET;
  by+=dy*FITOFFSET;

  // in case it didn't work (no key with mark, etc.)
  if (bx-ax <= 0 || by-ay <= 0)
  {
    ax = bax;
    ay = bay;
    bx = bbx;
    by = bby;
  }
}

void reset_frame() { reset_frame_m(MNONE); }


void center_frame_m(int val)
{
  // Center frame around keys with mark val (without changing scale).

  double bax=ax, bbx=bx, bay=ay, bby=by;
  double dx, dy;

  // first, we adujst frame
  reset_frame_m(val);

  // then, we restore previous scale around the new center
  dx = (bx + ax)/2 - (bbx + bax)/2;
  dy = (by + ay)/2 - (bby + bay)/2;

  ax = bax + dx;
  ay = bay + dy;
  bx = bbx + dx;
  by = bby + dy;
}

void center_frame() { center_frame_m(MNONE); }


void draw_rect(int x1, int y1, int x2, int y2, char r, char g, char b)
{
  // Draw a rectangle.

  int i;
  int xa, xb, ya, yb;
  
  SDL_LockSurface(screen);
  
  if (x1<x2) {xa = x1; xb = x2;} else {xa = x2; xb=x1;}
  if (y1<y2) {ya = y1; yb = y2;} else {ya = y2; yb=y1;}
  
  for (i = xa; i<=xb; i++)
  {
    pixel_draw(i, ya, r, g, b, SELECTION_RECT_SIZE);
    pixel_draw(i, yb, r, g, b, SELECTION_RECT_SIZE);
  }
  
  for (i = ya; i<=yb; i++)
  {
    pixel_draw(xa, i, r, g, b, SELECTION_RECT_SIZE);
    pixel_draw(xb, i, r, g, b, SELECTION_RECT_SIZE);
  
  }
  SDL_UnlockSurface(screen);
}


void redraw()
{
  // Redraw everything except the graph.
  
  int x, y;

  update_tooltip();
  update_status();
  update_input();

  if(mouse_byrect == 1)
  {
    // draw rectangle
    SDL_GetMouseState(&x, &y);
    draw_rect(x, y, mouse_x, mouse_y, 0, 80, 189);
  }

  SDL_Flip(screen);
}


void accel_draw(unsigned long i)
{
  // Draw acceleration for a key.
  // TODO: avoid magic numbers, find a more efficient way, antialiasing etc.

  unsigned long j;
  Uint32 *p;
  float adx, ady;
  int x, y;

  SDL_LockSurface(screen);

  for (j=0; j<10000; j++)
  {
    adx = ((get_kx(i)+j*get_kax(i)/1000-ax)/(bx-ax));
    ady = ((get_ky(i)+j*get_kay(i)/1000-ay)/(by-ay));

    x = (width-1)*adx;
    y = (height-1)*ady;

    if (valid_coord(x, y))
    {
      p = screen->pixels + y * screen->pitch + x * screen->format->BytesPerPixel;

      *p|=SDL_MapRGB(screen->format, 255, 0,0);
    }
  }

  SDL_UnlockSurface(screen);
}


void draw_caption(unsigned long i)
{
  // Draw caption for a key.
  // TODO 2: avoid overlapping, etc.

  char label[500];
  float adx, ady;
  int x, y;

  adx = ((get_kx(i)-ax)/(bx-ax));
  ady = ((get_ky(i)-ay)/(by-ay));
  x = (width-1)*adx;
  y = (height-1)*ady;

  if (valid_coord(x, y))
  {
    if (hasm(i,MI) && hasm(i,MN))
      sprintf((char *) &label, "%x - %s", vertices[i].id, vertices[i].name);
    else if (hasm(i, MI))
      sprintf((char *) &label, "%x", vertices[i].id);
    else
      sprintf((char *) &label, "%s", vertices[i].name);
    if (hasm(i, MU)) writeTxt(screen,label,x,y,white,small_font);
    else  writeTxt(screen,label,x,y,vertices[i].color,small_font);
  }
}


void apply_resize(SDL_Event event)
{
  width = event.resize.w;
  height = event.resize.h;
  printf("%d %d\n", width, height);
  reset_frame();
}


void graph_redraw(double ep)
{
  // Redraw graph.

  unsigned long i;
  float adx, ady;
  int x, y;

  char label[500];

  SDL_LockSurface(screen);
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

  for(i=0; i<num_keys; i++)
  {
    adx = ((get_kx(i)-ax)/(bx-ax));
    ady = ((get_ky(i)-ay)/(by-ay));
    x = (width-1)*adx;
    y = (height-1)*ady;

    if (valid_coord(x, y))
    {
      pixel_draw(x, y, vertices[i].color.r, vertices[i].color.g, vertices[i].color.b, DOT_SIZE);

      if (hasm(i, MU))
      pixel_draw(x, y, 0xff, 0xff, 0xff, DOT_SIZE);
    }
  }

  SDL_UnlockSurface(screen);

  do_all_m(&accel_draw, MA);
  do_all_m(&draw_caption, MI);
  do_all_m(&draw_caption, MN);

  if (ep)
  {
    sprintf((char*) &label, "EP: %le", ep);
    set_status(label);
    //printf("%s\n", eps);
  }
}


void redraw_all(double ep)
{
  graph_redraw(ep);
  redraw();
}


void in_circle(char* radius)
{
  unsigned long i;
  for (i=0;i<num_keys; i++)
    if (hasm(i, MU))
      circle(i, disp2real_x(atoi(radius)) - disp2real_x(0), disp2real_x(pointer_x), disp2real_y(pointer_y));
}


void opsel_by_color(char* color)
{
  unsigned long color_p;
  SDL_Color s_color;

  color_p = strtol(color, NULL, 16);
  s_color.r = color_p >> 16;
  s_color.g = ((color_p ^ (s_color.r << 16)) >> 8);
  s_color.b = (color_p ^ (s_color.r << 16)) ^ (s_color.g << 8 );
  color_m(s_color, MU);
  printf("Marked with color %x\n", color_p);
  delm_all(MV);
}

void color_m(SDL_Color color, char mark)
{
  // Color by mark.

  unsigned long i;
  for (i=0;i<num_keys; i++)
    if (hasm(i, mark))
      vertices[i].color = color;
}


void stop_input()
{
  inputting=0;
  if (tooltip == TOOLTIP_INPUT)
    remove_tooltip();
}


double disp2real_x(double disp_x)
{
  // converts an on-screen coordinate to a real coordinate
  return ax+(disp_x/width)*(bx-ax);
}

double disp2real_y(double disp_y)
{
  return ay+(disp_y/height)*(by-ay);
}


void zoom(int x, int y, double factor)
{
  ax = ax + (((double) bx - ax) * (((double) x) / ((double) width)) / factor);
  ay = ay + (((double) by - ay) * (((double) y) / ((double) height)) / factor);
  bx = bx - (((double) bx - ax) * (1. - (((double) x) / ((double) width))) / factor);
  by = by - (((double) by - ay) * (1. - (((double) y) / ((double) height))) / factor);
}


void help(char* msg) { set_tooltip(TOOLTIP_HELP, msg); }
void label(char* msg) { set_tooltip(TOOLTIP_INPUT, msg); }


void user_input(char* caption, void (*fun)(char*), char* initial, int initial_cursor)
{
  // Start user input.
  label(caption);
  input_callback = fun;
  inputting = 1;
  SDL_Input_SetText(ttf->input, initial);
  SDL_Input_SetCursorIndex(ttf->input, initial_cursor);
}


int writeTxt(SDL_Surface* screen, char *message, signed int x, signed int y, SDL_Color color, TTF_Font* font)
{
  // Write some text.

  int rslt=0;

  if(message != NULL)
  {
    SDL_Surface* txt = NULL;
    SDL_Rect position;

    txt = TTF_RenderText_Solid(font, message, color);
    position.x = x;
    position.y = y;
    SDL_BlitSurface(txt, NULL, screen, &position);

    SDL_FreeSurface(txt);
  }
  else rslt = 1;

  return rslt;
}


int progress_bar(double x)
{
  // Update progress bar.
  
  SDL_Event event;
  SDL_Surface *bar;

  printf("Progress is %lf.\n", x);
  if (x>=0 && x<=1)
  {
    bar = SDL_CreateRGBSurface(SDL_HWSURFACE, (int) (x * (width-1))+1, 20, 32, 0, 0, 0, 0);
    SDL_FillRect(bar, NULL, SDL_MapRGB(screen->format, 200, 200, 200));
    SDL_BlitSurface(bar, NULL, screen, &pos_status);
    SDL_FreeSurface(bar);
    SDL_Flip(screen);
  }
  else printf("ERROR: progress_bar value is not within [0, 1] range (got %lf)\n", x);
 
  // allow user to cancel
  while (SDL_PollEvent(&event))
    switch(event.type)
    {
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
          return (-1);
    }

  return 0;
}


void graphics_end()
{
  TTF_CloseFont(font);
  TTF_CloseFont(small_font);
  TTF_Quit();
  SDL_Quit();
}

