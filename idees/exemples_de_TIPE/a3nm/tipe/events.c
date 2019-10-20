#include "main.h"

// for rectangle selection
#define RECT_MAYBE 2
#define RECT_YES 1
#define RECT_NO 0


extern double ax, ay, bx, by; // frame

char inputting=0; // is user input currently taking place?
char moving=0; // is user currently moving keys around?
double delta_x, delta_y;

extern key vertices[MAXKEYS];

extern char tooltip;
extern char tooltip_time;
extern void *input_callback;

// when displaying info about a key which is close to the mouse pointer, temporarily store some information about this key
unsigned long last_key = 0;
char had_mn=0;
char had_mi=0;
SDL_Color had_color;

extern int width;
extern int height;

extern int auto_recalc;

// for rectangle selection
extern int mouse_x, mouse_y;
// to display rectangle on mouse move
extern char mouse_byrect;

// for mouse button drag and drop
extern int mouse_mx, mouse_my;
// matching real coordinates
extern double mouse_mx_r, mouse_my_r;
// to refresh on mouse move
extern char mouse_middle;

// Always contains up-to-date mouse coordinates.
int pointer_x, pointer_y;

extern char status[500];

extern unsigned long num_keys;

extern TTF_Font *font;
extern TTF_Font *small_font;

extern SDL_Surface *screen;
extern SDL_Input_TTF *ttf;
extern SDL_Rect inputpos;

extern int select_mode;

extern double disp2real_x(double disp_x);
extern double disp2real_y(double disp_y);
extern unsigned long find_closest_m(double x, double y, int val, double dist);
extern unsigned long find_closest(double x, double y, double dist);
extern void opsel_by_id_p(unsigned long id_p);
extern void opsel_by_region_p(double x1, double y1, double x2, double y2);

extern double total_dists(char printstep, unsigned long n_keys, unsigned long brk, float pb_f, float pb_t);
extern double total_gdists(char printstep, unsigned long n_keys, unsigned long brk, float pb_f, float pb_t);
extern void in_circle(char* radius);



int all_tlds_to_all_tlds_w(char* filename)
{
  // Callback function for all_tlds_to_all_tlds

  FILE* f;
  int rsl;

  f = fopen(filename, "w");
  if (!f)
  {
    help("Could not open file!\n");
    return 1;
  }

  help("Computing all_tlds_to_all_tlds...\n");

  rsl = all_tlds_to_all_tlds(0, f, 300);

  fclose(f);

  if (rsl == -1) help("Results of all_tlds_to_all_tlds saved.\n");
  else help("User abort.\n");

  return rsl;
}


int all_tlds_vs_rand_w(char* filename)
{
  // Callback function for all_tlds_vs_rand

  FILE* f;
  int rsl;

  f = fopen(filename, "w");
  if (!f)
  {
    help("Could not open file!\n");
    return 1;
  }

  help("Computing all_tlds_vs_rand...\n");

  rsl = all_tlds_vs_rand(0, f);

  fclose(f);

  if (rsl == -1) help("Results of all_tlds_vs_rand saved.\n");
  else help("User abort.\n");

  return rsl;
}


int manage_event()
{
  // Manage next SDL event.
  
  char msg[500];
  unsigned long nfrom, nto;
  double total_dist, total_gdist;

  int dx, dy;
  float ddx, ddy;
  unsigned long key;

  unsigned long color;

  SDL_Event event;

  if (!SDL_PollEvent(&event)) return 0; // no more events

  switch(event.type)
  {
    case SDL_KEYDOWN:
      if (inputting)
      {
        // an input field is currently displayed
        
        SDL_Input_TTF_Input( ttf, screen, &event );
        switch (event.key.keysym.sym)
        {
          case SDLK_RETURN:
            // send input to callback function
            printf("> %s\n", ttf->input->string);
            (*((void (*)(char*)) input_callback))(ttf->input->string);
            stop_input();
            break;

          case SDLK_ESCAPE:
            // cancel input
            printf("> [ESC]\n");
            stop_input();
            break;
        }
        redraw();

      } else {

        if (SDL_GetModState() & KMOD_CTRL)
        {
          switch (event.key.keysym.sym)
          {
            case SDLK_a:
              user_input("Please enter a file name to save the results of all_tlds_vs_rand:", &all_tlds_vs_rand_w, "all_tlds_vs_rand.csv", strlen("all_tlds_vs_rand"));
              break;

            case SDLK_b:
              user_input("Please enter a file name to save the results of all_tlds_to_all_tlds:", &all_tlds_to_all_tlds_w, "all_tlds_to_all_tlds.csv", strlen("all_tlds_to_all_tlds"));
              break;

            case SDLK_c:
              printf("%d\n", event.button.x);
              user_input("Please enter the circle radius in pixels:", &in_circle, "", 0);
          }
        }
        
        else if (SDL_GetModState() & KMOD_SHIFT)
        {
          switch (event.key.keysym.sym)
          {
            case SDLK_a:
              delm_all(MU); help("All keys unselected.");
              break;

            case SDLK_c:
              // TODO: intelligent autocolor
              color = rand() % (1 << 24);
              sprintf(msg, "0x%x", color);
              opsel_by_color(msg);
              sprintf(msg, "Marked with random color 0x%x.", color);
              help(msg);
              break;

            case SDLK_d:
              delm_all(MFT); help("From and To marks removed.");
              break;

            case SDLK_e: help("TODO: Select by name."); break;

            case SDLK_f:
              addm_selected(MT); help("Keys tagged as To.");
              break;

            case SDLK_g:
              delm_selected(MA); help("Key accel hidden for selected keys.");
              break;

            case SDLK_i:
              delm_selected(MI); help("Key IDs hidden for selected keys.");
              break;

            case SDLK_k: help("TODO: Select by color."); break;

            case SDLK_l:
              do_all(&update_key_accel);
              break;

            case SDLK_m:
              do_all(&move_key_step);
              break;

            case SDLK_n:
              delm_selected(MN); help("Keys names hidden for selected keys.");
              break;

            case SDLK_r: help("TODO: Intelligent random keys (excluding already selected if adding, etc.)"); break;

            case SDLK_s:
              addm_m_neighbours(MU, MV, FORWARDS);
              help("Key signees selected.");
              oprm_all(MU, MV,select_mode);
              delm_all(MV);
              break;

            case SDLK_u: help("TODO: redo."); break;

            case SDLK_w: help("TODO: save bitmap image."); break;

            case SDLK_x: help("TODO: set key redraw rate."); break;

            case SDLK_z:
              if (!selection_empty())
              {
                center_frame_m(MU);
                help("Zoom centered on selection");
              } else {
                center_frame_m(MNONE);
                help("Zoom centered on all keys.");
              }
              break;

            case SDLK_LSHIFT:
            case SDLK_RSHIFT:
            case SDLK_LCTRL:
            case SDLK_RCTRL:
            case SDLK_LALT:
            case SDLK_RALT:
            case SDLK_ESCAPE:
              // ignore
              break;

            default:
              help("No command bound to this key.");
              break;
          }
        } else {
          switch (event.key.keysym.sym)
          {
            case SDLK_a:
              addm_all(MU); help("All keys selected.");
              break;

            case SDLK_c:
              user_input("Please enter the color to use for selected keys (hexa):", &opsel_by_color, "0x", 2 );
              break;

            case SDLK_d:
              // compute distances
              nfrom = count_m(MF);
              nto = count_m(MT);

              if (nfrom && nto)
              {
                remove_tooltip();
                total_dist = total_dists(100, nfrom, 0, 0, 0.5);
                total_gdist = total_gdists(100,nfrom, 0, 0.5, 1);
                if (total_dist > 0 || total_gdist > 0)
                {
                  sprintf(msg, "Total dist : %lf, average dist %lf -- Total gdist : %lf, average gdist %lf -- gdist/dist=%lf", total_dist, total_dist/(nfrom*nto), total_gdist, total_gdist/(nfrom*nto), total_gdist/total_dist);
                } else {
                  sprintf(msg, "Abort.");
                }
              } else {
                sprintf(msg, "Select from and to keys with 'f' first.");
              }
              help(msg);
              break;

            case SDLK_e:
              user_input("Please enter a TLD:", &opsel_by_tld, ">", 0 );
              break;

            case SDLK_f:
              addm_selected(MF); help("Keys tagged as From.");
              break;

            case SDLK_g:
              addm_selected(MA); help("Key accel displayed for selected keys.");
              break;

            case SDLK_h: help("TODO: help."); break;  

            case SDLK_i:
              addm_selected(MI); help("Key IDs displayed for selected keys.");
              break;

            case SDLK_k:
              user_input("Please enter a key ID:", &opsel_by_id, "0x", 2);
              break;

            case SDLK_l:
              do_all_selected(&update_key_accel);
              break;

            case SDLK_m:
              do_all_m(&move_key_step, MU);
              break;

            case SDLK_n:
              addm_selected(MN); help("Keys names displayed for selected keys.");
              break;

            case SDLK_q:
              // exit
              return -1;
              break;

            case SDLK_r:
              user_input("Please enter the number of random keys to select:", &opsel_rand, "", 0);
              break;

            case SDLK_s:
              addm_m_neighbours(MU, MV, BACKWARDS);
              help("Key signers selected.");
              oprm_all(MU, MV,select_mode);
              delm_all(MV);
              break;

            case SDLK_t:
              sprintf(msg, "%lu keys out of %lu selected.", count_m(MU), num_keys);
              help(msg);
              break;

            case SDLK_u: help("TODO: undo."); break;

            case SDLK_v:
              notm_all(MU); help("Selection inverted.");
              break;

            case SDLK_w: help("TODO: save"); break;

            case SDLK_x:
              if (auto_recalc)
              {
                auto_recalc=0;  help("Auto_recalc disabled.");
              } else {
                auto_recalc=1;  help("Auto_recalc enabled.");
              }
              break;

            case SDLK_z:
              if (!selection_empty())
              {
                reset_frame_m(MU); help("Zoom fitted to selection.");
              } else {
                reset_frame_m(MNONE); help("Zoom fitted to all keys.");
              }
              break;


            case SDLK_SLASH:
              select_mode = SELECT_SET;
              help("Next selection commands will define selection.");
              break;

            case SDLK_PLUS:
              select_mode = SELECT_UNION;
              help("Next selection commands will be added to selection.");
              break;

            case SDLK_MINUS:
              select_mode = SELECT_REMOVE;
              help("Next selection commands will be removed from selection.");
              break;

            case SDLK_ASTERISK:
              select_mode = SELECT_INTERSECTION;
              help("Next selection commands will be intersected with selection.");
              break;

            case SDLK_BACKSLASH:
              select_mode = SELECT_XOR;
              help("Next selection commands will be XORed with selection.");
              break;


            case SDLK_WORLD_95:
              // TODO: for some weird reason, linking fails unless this command appears somewhere...
              SDL_Input_InputString(NULL, NULL, 0,0);
              break;

            case SDLK_LSHIFT:
            case SDLK_RSHIFT:
            case SDLK_LCTRL:
            case SDLK_RCTRL:
            case SDLK_LALT:
            case SDLK_RALT:
            case SDLK_ESCAPE:
              // ignore
              break;

            default:
              help("No command bound to this key.");
              break;
          }
        }
      }
      break;


    case SDL_MOUSEBUTTONDOWN:
      switch (event.button.button)
      {
        case SDL_BUTTON_RIGHT:
          // move keys
          if (!selection_empty())
          {
            // start moving current selection
            moving=1;
          } else {
            // we have no selection to move around
            if ((key = find_closest(disp2real_x(event.button.x), disp2real_y(event.button.y), MIN_DIST*(disp2real_x(1) - disp2real_x(0))*(disp2real_y(1) - disp2real_y(0)))) != num_keys)
            {
              // mouse pointer is close to a key, so we select it and start moving it
              opsel_by_id_p(vertices[key].id);
              moving = 2; // to remember that the key should be unselected when right button is released (it wasn't selected beforehand, because selection_empty() returned TRUE)
            } else {
              // no selection and no nearby key, so we can't start moving anything
            }
          }

          if (moving)
          {
            mouse_x = event.button.x;
            mouse_y = event.button.y;
          }

          break;


        case SDL_BUTTON_LEFT:
          // record current pointer coordinates
          mouse_x = event.button.x;
          mouse_y = event.button.y;
          // we don't know yet if user wants to draw a rectangle or select just one key
          mouse_byrect=RECT_MAYBE;
          break;


        case SDL_BUTTON_MIDDLE:
          mouse_mx = event.button.x;
          mouse_my = event.button.y;
          mouse_mx_r = ax;
          mouse_my_r = ay;
          
          // start moving view
          mouse_middle = 1;
        break;

        case SDL_BUTTON_WHEELDOWN:
          // zoom out
          zoom(event.button.x, event.button.y, -ZFACTOR);
          break;

        case SDL_BUTTON_WHEELUP:
          // zoom in
          zoom(event.button.x, event.button.y, ZFACTOR);
          break;

        default:
          help("No command bound to this button.");
          break;
      }
      break;

    case SDL_MOUSEBUTTONUP:
      switch (event.button.button)
      {
        case SDL_BUTTON_RIGHT:
          if (moving == 2) delm_all(MU); // unselect the key which was temporarily selected
          moving = 0; // stop moving keys
          break;

        case SDL_BUTTON_LEFT:

          dx = event.button.x - mouse_x;
          dy = event.button.y - mouse_y;

          if (mouse_byrect != RECT_YES)
          {
            // user selected one key
            // TODO: avoid already selected keys for operator union, etc.
            // find closest key and select it
            opsel_by_id_p(vertices[find_closest(disp2real_x(event.button.x), disp2real_y(event.button.y), MIN_DIST*(disp2real_x(1) - disp2real_x(0))*(disp2real_y(1) - disp2real_y(0)))].id);
          } else {
            // select all keys in region between pointer coordinates at MOUSEBUTTONDOWN and pointer coordinates at MOUSEBUTTONUP
            opsel_by_region_p(disp2real_x(event.button.x), disp2real_y(event.button.y), disp2real_x(mouse_x), disp2real_y(mouse_y));
          }

          mouse_byrect = RECT_NO;
          break;

        case SDL_BUTTON_MIDDLE:
          // stop moving view
          mouse_middle = 0;
          break;
      }
      break;

    case SDL_MOUSEMOTION:
      pointer_x = event.motion.x;
      pointer_y = event.motion.y;

      if (mouse_middle)
      {
        // change display position
        ddx = bx - ax;
        ddy = by - ay;

        ax -= -mouse_mx_r + disp2real_x(-mouse_mx + event.motion.x);
        ay -= -mouse_my_r + disp2real_y(-mouse_my + event.motion.y);

        bx = ax + ddx;
        by = ay + ddy;
      }

      if (moving)
      {
        // move keys around
        delta_x = - ((disp2real_x(mouse_x)) - disp2real_x(event.motion.x));
        delta_y = - ((disp2real_y(mouse_y)) - disp2real_y(event.motion.y));

        do_all_selected(&moving_adjust);

        mouse_x = event.motion.x;
        mouse_y = event.motion.y;
      }

      if (mouse_byrect == RECT_MAYBE)
      {
        dx = event.button.x - mouse_x;
        dy = event.button.y - mouse_y;

        if (dx*dx + dy*dy < SELECTION_DELTA)
          mouse_byrect = RECT_YES; // user has moved pointer too far, he probably intends a rectangle
      }

      if (!moving)
      {
        // display a tooltip
        key = find_closest(disp2real_x(event.motion.x), disp2real_y(event.motion.y), MIN_DIST*(disp2real_x(1) - disp2real_x(0))*(disp2real_y(1) - disp2real_y(0)));
        if (!had_mn) delm(last_key,MN);
        if (!had_mi) delm(last_key,MI);
        vertices[last_key].color = had_color;
        last_key = key;
        // we need to switch MN, MI and MU temporarily to allow for caption drawing
        had_mn = hasm(key, MN);
        had_mi = hasm(key, MI);
        had_color = vertices[key].color;
        addm(key, MN);
        addm(key, MI);
        vertices[key].color = white;
        draw_caption(key);
      }

      break;

    case SDL_VIDEORESIZE:
      apply_resize(event);
      break;

    case SDL_QUIT:
      // exit
      return -1;
      break;
  }

  return 1;
}

