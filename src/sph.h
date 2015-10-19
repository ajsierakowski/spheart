/* Written by Adam Sierakowski 2015
 * for JHU HEART: The Computer Laboratory
 *
 * Compile with: 'make'
 */

#ifndef _SPH_H
#define _SPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INDIR "../input"
#define OUTDIR "../output"
#define BUFLEN 256

// number of particles
int np;

// information for each particle
typedef struct part_struct {
  float x;
  float y;
  float z;
  float m;
  float Fx;
  float Fy;
  float Fz;
} part_struct;

// create pointer to particle list
part_struct *part;

#endif
