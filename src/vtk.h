/* Written by Adam Sierakowski 2015
 * for JHU HEART: The Computer Laboratory
 *
 * Compile with: 'make'
 */

#ifndef _VTK_H
#define _VTK_H

#include "sph.h"

// initialize output file
void init_vtk(void);

// write output file
void out_vtk(int stepnum, float time, int np, part_struct *part);

// write output file helper function
void part_out_vtk(int stepnum, int np, part_struct *part);

#endif
