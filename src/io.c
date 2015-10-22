/* Written by Adam Sierakowski 2015
 * for JHU HEART: The Computer Laboratory
 *
 * Compile with: 'make'
 */

#include "io.h"
#include "rng.h"
#include "sph.h"
#include "time.h"

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
  fret = fscanf(infile, "np %d\n", &np);
  fret = fscanf(infile, "duration %f\n", &duration);
  fret = fscanf(infile, "dt %f\n", &dt);

  // initialize current time and time step number
  ttime = 0.;
  stepnum = 0;

  // allocate particle list
  part = malloc(np * sizeof(part_struct));

  // read each particle configuration
  fret = fscanf(infile, "(x, y, z) m\n");
  for(i = 0; i < np; i++) {
    fret = fscanf(infile, "%f %f %f %f\n", &part[i].x, &part[i].y, &part[i].z,
      &part[i].m);
    // initialize forces, velocities, and accelerations equal to zero for now
    part[i].vx = 0.;
    part[i].vy = 0.;
    part[i].vz = 0.;
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
  printf("np = %d\n", np);
  printf("duration = %6.4f\n", duration);
  printf("dt = %6.4f\n", dt);
  for(i = 0; i < np; i++) {
    printf("part[%d]: (%7.4f, %7.4f, %7.4f), m = %7.4f\n", i,
      part[i].x, part[i].y, part[i].z, part[i].m);
  }
}

// randomly distribute particles and write configuration to file
void write_config_random(int np, float xs, float xe, float ys, float ye,
  float zs, float ze)
{
  int i;                    // iterator
  char loc[BUFLEN] = "";    // file location
  int fret = 0;             // fscanf return value
  fret = fret;              // prevent compiler warning

  float x, y, z, m;         // particle parameters to write

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
  fprintf(outfile, "np %d\n", np);
  fprintf(outfile, "duration 1.\ndt 0.01\n");
  fprintf(outfile, "(x, y, z) m\n");

  // write each particle configuration
  for(i = 0; i < np; i++) {
    // restrict within given domain size
    x  = (xe - xs) * rng_flt() + xs;
    y  = (ye - ys) * rng_flt() + ys;
    z  = (ze - zs) * rng_flt() + zs;
    m  = 1.;
    // write to file
    fprintf(outfile, "%9.6f %9.6f %9.6f %9.6f\n",
      x, y, z, m);
  }

  printf("Random configuration written to %s\n", loc);

  // close output config file
  fclose(outfile);
}

// free allocated space at the end
void clean_up(void)
{
  free(part);
}
