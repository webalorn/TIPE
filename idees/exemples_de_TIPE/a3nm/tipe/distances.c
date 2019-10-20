#include "main.h"


extern unsigned long num_keys;
extern double breadth_explore(int root, int o, int pprint);

extern double get_kx(unsigned long pos);
extern double get_ky(unsigned long pos);

char is_seen(unsigned long pos);



double get_dists(unsigned long pos)
{
  return breadth_explore(pos, BACKWARDS, 0);
}


double get_gdists(unsigned long pos)
{
  unsigned long j;
  double total_dist=0;
  double dx, dy;

  for (j=0; j<num_keys; j++)
    if(is_tmark(j))
    {
      // add distance between i and j
      dx = get_kx(pos) - get_kx(j);
      dy = get_ky(pos) - get_ky(j);
      total_dist+= sqrt(dx*dx+dy*dy);
    }

  return total_dist;
}


double calc_dists(char printstep, unsigned long n_keys, unsigned long brk, float pb_f, float pb_t, double (*get)(unsigned long))
{
  // Sums all distances between FROM keys and TO keys.
  // n_keys is passed so that marked nodes don't need to be counted again.
  // brk is used to approximate the result with only the brk first keys (obsolete).
  // pb_f and pb_t indicate the minimal and maximal progress bar values
  
  unsigned long i, ok_keys=0;
  double total_dist=0;

  for (i=0; i<num_keys; i++)
    if(is_fmark(i))
    {
      // count all distances from this key
      total_dist+=(*get)(i);
      ok_keys++;

      if (printstep && !(ok_keys % printstep))
      {
        // display status every now and then, and update progress bar
        printf("Checked %lu keys, total dist is %lf, progress is %lf between %lf and %lf.\n", i, total_dist, pb_f + ( ( (double) ok_keys)/n_keys )*(pb_t - pb_f), pb_f, pb_t);
        if (progress_bar((pb_f + ( ( (double) ok_keys)/n_keys )*(pb_t - pb_f))))
          return -(1.0);
      }
      if (brk && ok_keys>brk) break;
    }
  if (brk && ok_keys>brk)
    return (total_dist*n_keys)/ok_keys; // estimate
  else return total_dist; // exact result
}


double total_dists(char printstep, unsigned long n_keys, unsigned long brk, float pb_f, float pb_t)
{
  // Sums all graph distances between FROM keys and TO keys.
  return calc_dists(printstep, n_keys, brk, pb_f, pb_t, &get_dists);
}


double total_gdists(char printstep, unsigned long n_keys, unsigned long brk, float pb_f, float pb_t)
{
  // Sums all distances between FROM keys and TO keys in graphical representation.
  return calc_dists(printstep, n_keys, brk, pb_f, pb_t, &get_gdists);
}


double dist_between_tlds(char *tld1, char* tld2, char precision, float pb_f, float pb_t)
{
  // Computes total distance between two TLDs.
  
  unmark_all();

  fmark_by_tld(tld1); tmark_by_tld(tld2);

  unsigned long n_fkeys=count_fmarks(), n_tkeys=count_tmarks();

  printf("For the %lu keys in TLD1 %s to the %lu keys in TLD2 %s: computing...\n", n_fkeys, tld1, n_tkeys, tld2);

  double dist = total_dists(DISPRATE,n_fkeys,precision, pb_f, pb_t);

  printf("For the %lu keys in TLD1 %s to the %lu keys in TLD2 %s: %lf (precision %d)\n", n_fkeys, tld1, n_tkeys, tld2, dist/(n_fkeys*n_tkeys), precision);

  unmark_all();

  return dist;
}


double fdist_between_tld_and_rand(char *tld, unsigned long num, char precision, float pb_f, float pb_t)
{
  // Computes distance from TLD to random set.

  unmark_all();

  fmark_by_tld(tld); tmark_rand(num);

  unsigned long n_fkeys=count_fmarks(), n_tkeys=count_tmarks();

  double dist = total_dists(DISPRATE,n_fkeys,precision, pb_f, pb_t);

  printf("For the %lu keys in TLD1 %s TO %lu random keys: %lf (precision %d)\n", n_fkeys, tld, n_tkeys, dist/(n_fkeys*n_tkeys), precision);

  unmark_all();

  return dist;
}


double tdist_between_tld_and_rand(char *tld, unsigned long num, char precision, float pb_f, float pb_t)
{
  // Computes distance from random set to TLD.

  unmark_all();

  tmark_by_tld(tld); fmark_rand(num);

  unsigned long n_fkeys=count_fmarks(), n_tkeys=count_tmarks();

  double dist = total_dists(DISPRATE,n_fkeys,precision, pb_f, pb_t);

  printf("For the %lu keys in TLD1 %s FROM %lu random keys: %lf (precision %d)\n", n_tkeys, tld, n_fkeys, dist/(n_fkeys*n_tkeys), precision);

  unmark_all();

  return dist;
}


double dist_between_rand(unsigned long num1, unsigned long num2, char precision, float pb_f, float pb_t)
{
  // Computes distance between random sets.
  
  unmark_all();

  fmark_rand(num1); tmark_rand(num2);

  double dist = total_dists(DISPRATE,num1,precision, pb_f, pb_t);

  printf("For %lu random keys to %lu random keys: %lf (precision %d)\n", num1, num2,dist/(num1*num2), precision);

  unmark_all();

  return dist;
}



void two_tlds_vs_rand(char *tld1, char*tld2, char precision)
{
  // Compares two TLDs.

  unsigned long n_fkeys=count_tld(tld1), n_tkeys=count_tld(tld2);

  dist_between_tlds(tld1, tld2, precision, 0, 1);
  fdist_between_tld_and_rand(tld1, n_tkeys, precision, 0, 1);
  tdist_between_tld_and_rand(tld2, n_fkeys, precision, 0, 1);
  dist_between_rand(n_fkeys, n_tkeys, precision, 0, 1);
}


double dist_of_tld(char* tld, char precision, float pb_f, float pb_t)
{
  // Computes distance within a TLD.

  return dist_between_tlds(tld, tld, precision, pb_f, pb_t);
}


double dist_of_rand(unsigned long num, char precision, float pb_f, float pb_t)
{
  // Computes distance within a random set.
  // This is not the same thing as dist_between_rand(x, x, ...) which would compare two different random sets of the same size.
  
  unmark_all();

  mark_rand(num);

  double dist = total_dists(DISPRATE,num,precision, pb_f, pb_t);

  printf("For %lu random keys: %lf (precision %d)\n", num, dist/(num*num), precision);

  unmark_all();

  return dist;
}


int all_tlds_vs_rand(char precision, FILE* out)
{
  // Compares all TLDs to random sets, and outputs the results to a file.
  
  unsigned long i, num, seen=0;
  double dist1, dist1g, dist2, dist2g, alldist, alldistg;
  char tld[10], str[2000];

  // MS is used to mark keys from TLDs we have already seen
  delm_all(MS);

  // headers
  if (out) fprintf(out, "TLD;Nombre;Dist. TLD;Dist. graph. TLD;Dist. rand.;Dist. graph. rand;Diff.;Diff. graph.\n");

  for (i=0;i<num_keys;i++)
  {
    if(!is_seen(i))
    {
      tld[0] = '\0';
      get_tld(i, &tld, 8);
      if(tld[0] == '.') // ignore garbage TLDS
      {
        // examine this TLD
        printf("TLD %s, index %lu\n", tld, i);

        add_by_tld(&tld,MS);

        num = count_tld(tld);

        // distance within TLD
        dist1 = dist_of_tld((char*) &tld, precision, 0.25*((float) seen / (float) (num_keys+1)), 0.25*((float) seen / (float) (num_keys+1)) + .5*((float) num / (2 * (float) (num_keys+1))) );
        if (dist1 < 0) // user cancel
          return -1;

        unmark_all();
        mark_by_tld(tld);
        dist1g = total_gdists(DISPRATE, num, precision, 0.25*((float) seen / (float) (num_keys+1)) + 0.5*((float) num / (2 * (float) (num_keys+1))), 0.25*((float) seen / (float) (num_keys+1)) + ((float) num / (2 * (float) (num_keys+1))) );
        unmark_all();
        if (dist1g < 0) // user cancel
          return -1;

        // distance within random set
        dist2 = dist_of_rand(num, precision, 0.25*((float) seen / (float) (num_keys+1)) + ((float) num / (2 * (float) (num_keys+1))), 0.25*((float) seen / (float) (num_keys+1)) + 0.75*((float) num) / ((float) (num_keys+1)));
        if (dist2 < 0) // user cancel
          return -1;

        mark_rand(num);
        dist2g = total_gdists(DISPRATE, num, precision, 0.25*((float) seen / (float) (num_keys+1)) + 0.75*((float) num / ((float) (num_keys+1))), 0.25*((float) seen / (float) (num_keys+1)) + ((float) num / ( (float) (num_keys+1))) );
        unmark_all();
        if (dist2g < 0) // user cancel
          return -1;


        strip_gt(&tld);
        sprintf(str, "%s;%ld;%lf;%lf;%lf;%lf;%lf;%lf\n", tld, num, dist1/((double) num*num), dist1g/((double) num*num), dist2/((double) num*num), dist2g/((double) num*num), (dist2-dist1)/((double) num*num), (dist2g-dist1g)/((double) num*num));
        printf("%s", str); if (out) fprintf(out, "%s", str);
        seen += num;
      }
    }
  }

  // grand total
  
  mark_all();

  alldist = total_dists(DISPRATE, num_keys, 0, .5, .75);
  alldistg = total_gdists(DISPRATE, num_keys, 0, .75, 1);

  sprintf(str, "%s;%ld;%lf;%lf;%lf;%lf;%lf;%lf\n", "TOTAL", num_keys, alldist/((double) num_keys*num_keys), alldistg/((double) num_keys*num_keys), alldist/((double) num_keys*num_keys),alldistg/((double) num_keys*num_keys), 0, 0);
  printf("%s", str); if (out) fprintf(out, "%s", str);

  unmark_all();

  return 0;
}


unsigned long all_tlds_header(FILE* out, unsigned long min)
{
  // Create header.
  
  unsigned long i, num, total_keys=0;
  char tld[10];

  delm_all(MS);
  printf(";", tld); if (out) fprintf(out, ";", tld);

  for (i=0;i<num_keys;i++)
  {
    if(!is_seen(i))
    {
      tld[0] = '\0';
      get_tld(i, &tld, 8);
      if(tld[0] == '.')
      {
        add_by_tld(&tld,MS);
        num = count_tld(tld);
        if (num >= min) // ignore small TLDS
        {
          printf("header: TLD %s, index %lu, num %lu\n", tld, i, num);
          strip_gt(&tld);
          printf(";%s\n", tld); if (out) fprintf(out, ";%s", tld);
          total_keys += num;
        }
      }
    }
  }

  // end of header
  printf("\n"); if (out) fprintf(out, "\n");

  delm_all(MS);

  return total_keys;
}



int all_tlds_to_all_tlds(char precision, FILE* out, unsigned long min)
{
  // Compares all TLDs to each other.

  unsigned long i, j, num, num2, seen=0, seen2=0, total_keys=0;
  double dist;
  char tld[10], tlds[10], tld2[10];

  total_keys = all_tlds_header(out, min);

  // MS and MU are used
  delm_all(MS);

  for (i=0;i<num_keys;i++)
  {
    if(!is_seen(i))
    {
      delm_all(MU);
      tld[0] = '\0';
      get_tld(i, &tld, 8);
      if(tld[0] == '.')
      {
        num = count_tld(tld);
        add_by_tld(&tld,MS);
        if (num >= min ) // ignore small TLDS
        {
          printf("TLD %s, index %lu\n", tld, i);
          strcpy(tlds, tld);
          strip_gt(tlds);
          if (out) fprintf(out, "%s", tlds);
          seen2=0;

          for (j=0;j<num_keys;j++)
          {
            if(!hasm(j, MU))
            {
              tld2[0] = '\0';
              get_tld(j, &tld2, 8);
              if(tld2[0] == '.')
              {
                num2 = count_tld(tld2);
                add_by_tld(&tld2,MU);
                if (num2 >= min) // ignore small TLDS
                {
                  // compare TLDs tld and tld2

                  printf("- TLD %s, index %lu\n", tld2, j);

                  // TODO progress bar is wrong
                  dist = dist_between_tlds((char*) &tld, (char*) &tld2, precision, (((float) seen) / total_keys) + (((float) num * seen2) / (total_keys * total_keys)), (((float) seen) / total_keys) + (((float) num * (seen2 + num2)) / (total_keys * total_keys)));
                  if (dist < 0) // user cancel
                    return -1;

                  strip_gt(&tld2);
                  printf("from %s to %s: %lf total, %lf average\n", tld, tld2, dist, dist/((double) num*num2));
                  if (out) fprintf(out, ";%.2lf", tld, dist/((double) num*num2));
                  seen2 += num;
                }
              }
            }
          }

          if (out) fprintf(out, ";%s", tlds);
          seen += num;

          printf("\n"); if (out) fprintf(out, "\n");
        }
      }
    }
  }

  delm_all(MU);

  all_tlds_header(out, min);

  return 0;
}

