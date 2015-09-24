/* Written by Adam Sierakowski 2015
 * for JHU HEART: The Computer Laboratory
 *
 * Compile with: 'gcc src/sph.c -o sph' from ../
 */

#include <stdio.h>
#include <stdlib.h>

#define BUFLEN 256
#define INDIR "input"

int main(int argc, char *argv[]) {
  int np;   // number of particles

  // particle data
  typedef struct part_struct{
    int n;
    float x;
    float y;
    float z;
    float a;
  } part_struct;

  int i;    // iterator

  // open configuration file
  char loc[BUFLEN] = "";  // file location
  sprintf(loc, "%s/config", INDIR);
  FILE *infile = fopen(loc, "r");
  if(infile == NULL) {    // check that it opened
    fprintf(stderr, "Could not open file %s\n", loc);
    exit(EXIT_FAILURE);
  }

  int fret = 0;   // for fscanf return value
  fret = fret;    // prevent compiler warning

  fret = fscanf(infile, "np %d\n", &np);
  fret = fscanf(infile, "(x, y, z) a\n");

  // allocate particle list
  part_struct *part = malloc(np * sizeof(part_struct));

  // read information in config to part_struct
  for(i = 0; i < np; i++) {
    part[i].n = i;
    fret = fscanf(infile, "%f %f %f %f\n", &part[i].x, &part[i].y, &part[i].z,
      &part[i].a);
  }

  // write information in part_struct to screen
  for(i = 0; i < np; i++) {
    printf("n = %d, (%f, %f, %f), a = %f\n", part[i].n, part[i].x, part[i].y,
      part[i].z, part[i].a);
  }

  // clean up
  fclose(infile);
  free(part);
  
  return EXIT_SUCCESS;
}
