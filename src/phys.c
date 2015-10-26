/* Written by Adam Sierakowski 2015
 * for JHU HEART: The Computer Laboratory
 *
 * Compile with: 'make'
 */

#include "sph.h"
#include "phys.h"

void compute_forces(void) {
  int i, j;               // iterators
  float rx, ry, rz, r;    // distance vector components and magnitude

  for(i = 0; i < np; i++) {
    part[i].Fx = 0.;
    part[i].Fy = 0.;
    part[i].Fz = 0.;
    for(j = 0; j < np; j++) {
      if(i != j) {
        rx = part[j].x - part[i].x;
        ry = part[j].y - part[i].y;
        rz = part[j].z - part[i].z;
        r  = sqrt(rx*rx + ry*ry + rz*rz);
        if(r < 1.e-6) r = 1.e-6;

        part[i].Fx += (rx / r) * G * part[i].m * part[j].m / (r*r);
        part[i].Fy += (ry / r) * G * part[i].m * part[j].m / (r*r);
        part[i].Fz += (rz / r) * G * part[i].m * part[j].m / (r*r);
      }
    }
  }
}

void integrate_motion(void) {
  int i;                // iterator

  for(i = 0; i < np; i++) {
    part[i].ax = part[i].Fx / part[i].m;
    part[i].ay = part[i].Fy / part[i].m;
    part[i].az = part[i].Fz / part[i].m;

    part[i].vx = part[i].vx + dt * part[i].ax;
    part[i].vy = part[i].vy + dt * part[i].ay;
    part[i].vz = part[i].vz + dt * part[i].az;

    part[i].x = part[i].x + dt * part[i].vx;
    part[i].y = part[i].y + dt * part[i].vy;
    part[i].z = part[i].z + dt * part[i].vz;

/*
    if(part[i].x < -10) {
      part[i].x = -10;
    } else if(part[i].x > 10) {
      part[i].x = 10;
    }
    if(part[i].y < -10) {
      part[i].y = -10;
    } else if(part[i].y > 10) {
      part[i].y = 10;
    }
    if(part[i].z < -10) {
      part[i].z = -10;
    } else if(part[i].z > 10) {
      part[i].z = 10;
    }
*/
  }
}
