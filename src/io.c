/* Written by Adam Sierakowski 2015
 * for JHU HEART: The Computer Laboratory
 *
 * Compile with: 'make'
 */

#include "io.h"
#include "sph.h"

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

  // allocate particle list
  part = malloc(np * sizeof(part_struct));

  // read each particle configuration
  fret = fscanf(infile, "(x, y, z) m\n");
  for(i = 0; i < np; i++) {
    fret = fscanf(infile, "%f %f %f %f\n", &part[i].x, &part[i].y, &part[i].z,
      &part[i].m);
    // initialize forces equal to zero for now
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
  for(i = 0; i < np; i++) {
    printf("part[%d]: (%7.4f, %7.4f, %7.4f), m = %7.4f\n", i,
      part[i].x, part[i].y, part[i].z, part[i].m);
  }
}

// free allocated space at the end
void clean_up(void)
{
  free(part);
}
