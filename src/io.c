/* Written by Adam Sierakowski 2015
 * for JHU HEART: The Computer Laboratory
 *
 * Compile with: 'make'
 */

#include "io.h"
#include "rng.h"
#include "sph.h"
#include "time.h"
#include <math.h>

// read configuration from file
void read_config(void)
{
  int i;                    // iterator
  char loc[BUFLEN] = "";    // file location
  int fret = 0;             // fscanf retun value
  fret = fret;              // prevent compiler warning

  // open file
  sprintf(loc, "%s/config", INDIR);
  FILE *infile = fopen(loc, "r");
  if(infile == NULL) {
    fprintf(stderr, "Could not open file %s\n", loc);
    exit(EXIT_FAILURE);
  }

  // begin reading config file
  fret = fscanf(infile, "(xs, ys, zs) %f %f %f\n", &xs, &ys, &zs);
  fret = fscanf(infile, "(xe, ye, ze) %f %f %f\n", &xe, &ye, &ze);
  fret = fscanf(infile, "h %f\n", &h);
  fret = fscanf(infile, "np %d\n", &np);
  fret = fscanf(infile, "duration %f\n", &duration);
  fret = fscanf(infile, "dt %f\n", &dt);

  // initialize current time and time step number
  ttime = 0.;
  stepnum = 0;

  // allocate particle list
  part = malloc(np * sizeof(part_struct));

  // read each particle configuration
  fret = fscanf(infile, "(x, y, z) r m (vx, vy, vz) E\n");
  for(i = 0; i < np; i++) {
    fret = fscanf(infile, "%f %f %f %f %f %f %f %f %f\n",
      &part[i].x, &part[i].y, &part[i].z,
      &part[i].r, &part[i].m,
      &part[i].vx, &part[i].vy, &part[i].vz, &part[i].E);
    // initialize forces and accelerations equal to zero for now
    part[i].ax = 0.;
    part[i].ay = 0.;
    part[i].az = 0.;
    part[i].Fx = 0.;
    part[i].Fy = 0.;
    part[i].Fz = 0.;
  }

  // close config file
  fclose(infile);
}

// write configuration to stdout
void show_config(void)
{
  int i;                    // iterator

  // write configuration to stdout
  printf("(xs, ys, zs) = (%7.4f, %7.4f, %7.4f)\n", xs, ys, zs);
  printf("(xe, ye, ze) = (%7.4f, %7.4f, %7.4f)\n", xe, ye, ze);
  printf("h = %7.4f\n", h);
  printf("np = %d\n", np);
  printf("duration = %6.4f\n", duration);
  printf("dt = %6.4f\n", dt);
  for(i = 0; i < np; i++) {
    printf("part[%d]: (%7.4f, %7.4f, %7.4f), r = %7.4f, m = %7.4f, E = %7.4f",
      i, part[i].x, part[i].y, part[i].z, part[i].r, part[i].m, part[i].E);
    printf(", (%7.4f, %7.4f, %7.4f)\n", part[i].vx, part[i].vy, part[i].vz);
  }
}

// randomly distribute particles and write configuration to file
void write_config_random(int np, float r, float m, float E,
  float xs, float xe, float ys, float ye, float zs, float ze)
{
  int i, j;                           // iterator
  char loc[BUFLEN] = "";              // file location
  int fret = 0;                       // fscanf return value
  fret = fret;                        // prevent compiler warning

  // set random number generator seed
  rng_init(time(NULL));

  // open output config file
  sprintf(loc, "%s/config.rand", INDIR);
  FILE *outfile = fopen(loc, "w");
  if(outfile == NULL) {
    fprintf(stderr, "Could not open file %s\n", loc);
    exit(EXIT_FAILURE);
  }

  // write number of particles and header line
  fprintf(outfile, "(xs, ys, zs) %f %f %f\n", xs, ys, zs);
  fprintf(outfile, "(xe, ye, ze) %f %f %f\n", xe, ye, ze);
  float H = (xe-xs + ye-ys + ze-zs) / 3.;
  fprintf(outfile, "h %f\n", H);
  fprintf(outfile, "np %d\n", np);
  fprintf(outfile, "duration 1.\ndt 0.01\n");
  fprintf(outfile, "(x, y, z) r m (vx, vy, vz) E\n");

  // make a temporary particle structure
  part_struct *part_out = malloc(np * sizeof(part_struct));

  // write each particle configuration
  for(i = 0; i < np; i++) {
    printf("Placing particle number %d of %d\n", i+1, np);
    int intersect = 0;
    while(!intersect) {
      // restrict within given domain size
      part_out[i].x  = (xe - xs) * rng_flt() + xs;
      part_out[i].y  = (ye - ys) * rng_flt() + ys;
      part_out[i].z  = (ze - zs) * rng_flt() + zs;
      part_out[i].r  = r;
      part_out[i].m  = m;
      part_out[i].vx = rng_flt() - 0.5;
      part_out[i].vy = rng_flt() - 0.5;
      part_out[i].vz = rng_flt() - 0.5;
      part_out[i].E = E;

      intersect = !intersect;

      // check whether this particle interferes with any others
      for(j = 0; j < i; j++) {
        float r=sqrt((part_out[j].x-part_out[i].x)*(part_out[j].x-part_out[i].x)
                 + (part_out[j].y-part_out[i].y)*(part_out[j].y-part_out[i].y)
                 + (part_out[j].z-part_out[i].z)*(part_out[j].z-part_out[i].z));
        if(r < (part_out[i].r + part_out[j].r)) {
          intersect = !intersect;
          break;
        }
      }
    }
  }

  // write to file
  for(i = 0; i < np; i++) {
    fprintf(outfile, "%9.6f %9.6f %9.6f %9.6f %9.6f %9.6f %9.6f %9.6f %9.6f\n",
      part_out[i].x, part_out[i].y, part_out[i].z, part_out[i].r, part_out[i].m,
      part_out[i].vx, part_out[i].vy, part_out[i].vz, part_out[i].E);
  }

  free(part_out);

  printf("Random configuration written to %s\n", loc);

  // close output config file
  fclose(outfile);
}

// free allocated space at the end
void clean_up(void)
{
  free(part);
}
