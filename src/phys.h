/* Written by Adam Sierakowski 2015
 * for JHU HEART: The Computer Laboratory
 *
 * Compile with: 'make'
 */

#ifndef _PHYS_H
#define _PHYS_H

#include <math.h>

#define G 6.67408e-11   // gravitational constant (m^3 * kg^-1 * s^-2)

// compute forces (gravity)
void compute_forces(void);

// integrate particle motion
void integrate_motion(void);

#endif
