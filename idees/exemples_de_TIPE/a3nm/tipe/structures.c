#include "main.h"

extern key vertices[MAXKEYS];

extern struct sig sigs[MAXTSIGS];
extern unsigned int num_sigs;
extern unsigned long num_keys;

char colors[MAXKEYS];
char depths[MAXKEYS];

extern signatures edges[MAXKEYS];
extern signatures redges[MAXKEYS];

void enqueue(unsigned long pos, queue* q)
{
  q->q[q->t] = pos;
  q->t++;
  if (q->t == MAXSIGS)
    q->t = 0;
  if (q->h == q->t)
  {
    printf("Error: queue overflow.\n");
    exit(ERR_QOF);
  }
}

unsigned long dequeue(queue* q)
{
  unsigned long rsl;
  if (q->h == q->t)
  {
    printf("Error: queue underflow.\n");
    exit(ERR_QUF);
  }
  rsl = q->q[q->h];
  q->h++;
  if (q->h == MAXSIGS)
    q->h = 0;
  return rsl;
}

int queue_empty(queue* q)
{
  return q->h==q->t;
}

void queue_reset(queue* q)
{
  q->h=q->t=0;
}


char is_empty(signatures s)
{
  return s.head==NULL;
}


void push(signatures *stack, struct signature *s)
{
  if (is_empty(*stack))
  {
    stack->last = s;
    stack->head = s;
  } else {
    stack->last->next = s;
    stack->last = s;
  }
}


int next(struct sig* s, int orientation)
{
  // Find next key, according to orientation.

  if (orientation == FORWARDS)
  {
    return s->t;
  } else {
    return s->f;
  }
}

void breadth_explore_init(unsigned long i)
{
  colors[i] = WHITE;
  depths[i] = -1;
}


double breadth_explore(int root, int o, int pprint)
{
  unsigned long current;
  struct signature * s;
  int depth = 0;
  double dist = 0;
  queue q;

  do_all(&breadth_explore_init);

  colors[root] = GRAY;
  depths[root] = 0;

  queue_reset(&q);
  enqueue(root, &q);

  while(!queue_empty(&q))
  {
    current = dequeue(&q);

    if (o == BACKWARDS)
      s = edges[current].head;
    else
      s = redges[current].head;

    while (s)
    {
      if (colors[next(s->s,o)] == WHITE)
      {
        if (depth == depths[current])
          depth++;

        colors[next(s->s,o)] = GRAY;
        depths[next(s->s,o)] = depths[current]+1;

        if(is_tmark(next(s->s, o)))
          dist+=depth;

        enqueue(next(s->s, o), &q);
      }
      s = s->next;
    }
    colors[current] = BLACK;
  }

  return dist;
}

