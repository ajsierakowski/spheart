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

        if(r > (part[i].r + part[j].r)) {
          part[i].Fx += (rx / r) * G * part[i].m * part[j].m / (r*r);
          part[i].Fy += (ry / r) * G * part[i].m * part[j].m / (r*r);
          part[i].Fz += (rz / r) * G * part[i].m * part[j].m / (r*r);
        } else {
          part[i].Fx -= (rx / r) * part[i].E * fabs(part[i].r - r);
          part[i].Fy -= (ry / r) * part[i].E * fabs(part[i].r - r);
          part[i].Fz -= (rz / r) * part[i].E * fabs(part[i].r - r);
        }
      }
    }
  }
}

void integrate_motion(void) {
  int i;                // iterator

  float ax, ay, az;     // temporary velocity storage
  float vx, vy, vz;     // temporary velocity storage

  for(i = 0; i < np; i++) {
    ax = part[i].Fx / part[i].m;
    ay = part[i].Fy / part[i].m;
    az = part[i].Fz / part[i].m;

    vx = part[i].vx + dt * (1.5 * ax - 0.5 * part[i].ax);
    vy = part[i].vy + dt * (1.5 * ay - 0.5 * part[i].ay);
    vz = part[i].vz + dt * (1.5 * az - 0.5 * part[i].az);

    part[i].x += dt * (1.5 * vx - 0.5 * part[i].vx);
    part[i].y += dt * (1.5 * vy - 0.5 * part[i].vy);
    part[i].z += dt * (1.5 * vz - 0.5 * part[i].vz);

    // store acceleration and velocity
    part[i].ax = ax;
    part[i].ay = ay;
    part[i].az = az;

    part[i].vx = vx;
    part[i].vy = vy;
    part[i].vz = vz;

    if(part[i].x < xs) {
      part[i].vx = -part[i].vx;
    } else if(part[i].x > xe) {
      part[i].vx = -part[i].vx;
    }
    if(part[i].y < ys) {
      part[i].vy = -part[i].vy;
    } else if(part[i].y > ye) {
      part[i].vy = -part[i].vy;
    }
    if(part[i].z < zs) {
      part[i].vz = -part[i].vz;
    } else if(part[i].z > ze) {
      part[i].vz = -part[i].vz;
    }
  }
}
