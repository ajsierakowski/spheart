/* Written by Adam Sierakowski 2015
 * for JHU HEART: The Computer Laboratory
 *
 * Compile with: 'make'
 */

#include "sph.h"
#include "phys.h"
#include "mergesort.h"

// compute forces (gravity)
void compute_forces(void) {
  int l, m;               // iterators
  int i, j, k;            // particle bin indices
  int ii, jj, kk;         // interaction particle bin indices
  float rx, ry, rz, r;    // distance vector components and magnitude
  int n;                  // temporary bin location
  int I, J;               // particle interaction indices

  // set up bin discretization
  float Lx = xe - xs;
  float Ly = ye - ys;
  float Lz = ze - zs;
  int bnx = ceil(Lx / h);
  int bny = ceil(Ly / h);
  int bnz = ceil(Lz / h);
  float bdx = Lx / (float) bnx;
  float bdy = Ly / (float) bny;
  float bdz = Lz / (float) bnz;

  int *pnum = malloc(np * sizeof(int)); // list of particle numbers
  int *pbin = malloc(np * sizeof(int)); // corresponding list of particle bins
  int *bind = malloc(bnx*bny*bnz * sizeof(int));  // starting particle indices

  // loop through particles and put them into bins
  for(l = 0; l < np; l++) {
    pnum[l] = l;
    i = floor((part[l].x - xs) / bdx);
    j = floor((part[l].y - ys) / bdy);
    k = floor((part[l].z - zs) / bdz);

    if(i >= bnx) i = bnx - 1;
    if(i < 0) i = 0;
    if(j >= bny) j = bny - 1;
    if(j < 0) j = 0;
    if(k >= bnz) k = bnz - 1;
    if(k < 0) k = 0;

    pbin[l] = i + j*bnx + k*bnx*bny;
  }

  // sort by pbin
  merge_sort(pbin, np, pnum);

  // set up starting index list
  for(l = 0; l < bnx*bny*bnz; l++) {
    bind[l] = -1;
  }
  // determine starting index for each bin
  bind[pbin[0]] = 0;
  for(l = 1; l < np; l++) {
    if(pbin[l] > pbin[l-1]) { // if the bin number changes
      bind[pbin[l]] = l;      // store the bin number
    }
  }

  // interact particles in this and surrounding bins
  for(l = 0; l < np; l++) { // visit all particles
    // determine bin coordinates
    k = pbin[l] / (bnx*bny);
    j = (pbin[l] - k*bnx*bny) / bnx;
    i = pbin[l] - j*bnx - k*bnx*bny;
    I = pnum[l];  // this particle number

    // set sum of forces equal to zero
    part[I].Fx = 0.;
    part[I].Fy = 0.;
    part[I].Fz = 0.;

    // visit all sourrounding bins
    for(kk = k-1; kk <= k+1; kk++) {
      for(jj = j-1; jj <= j+1; jj++) {
        for(ii = i-1; ii <= i+1; ii++) {
          // make sure neighbors exist
          if(  ii >= 0 && ii < bnx
            && jj >= 0 && jj < bny
            && kk >= 0 && kk < bnz) {

            n = ii + jj*bnx + kk*bnx*bny;   // cell number

            m = bind[n];
            if(m > -1) {
              while(pbin[m] == pbin[bind[n]]) {
                J = pnum[m];  // other particle number

                // do not interact with self
                if(I != J) {
                  // separation vector
                  rx = part[J].x - part[I].x;
                  ry = part[J].y - part[I].y;
                  rz = part[J].z - part[I].z;
                  // modulus of separation vector
                  r  = sqrt(rx*rx + ry*ry + rz*rz);

                  // if not contacting, compute force due to gravity
                  if(r > (part[I].r + part[J].r)) {
                    part[I].Fx += (rx / r) * G * part[I].m * part[J].m / (r*r);
                    part[I].Fy += (ry / r) * G * part[I].m * part[J].m / (r*r);
                    part[I].Fz += (rz / r) * G * part[I].m * part[J].m / (r*r);
                  } else {  // if contacting, apply spring force to separate
                    part[I].Fx -= (rx / r) * part[I].E * fabs(part[I].r - r);
                    part[I].Fy -= (ry / r) * part[I].E * fabs(part[I].r - r);
                    part[I].Fz -= (rz / r) * part[I].E * fabs(part[I].r - r);
                  }
                }
                m++;  // move to the next particle in the bin

                // exit loop if the next m is one too far
                if(m == np) {
                  break;
                }
              }
            }
          }
        }
      }
    }
  }

  free(pnum);
  free(pbin);
  free(bind);
}

// integrate particle motion
void integrate_motion(void) {
  int i;                // iterator

  float ax, ay, az;     // temporary velocity storage
  float vx, vy, vz;     // temporary velocity storage

  // loop over all particles
  for(i = 0; i < np; i++) {
    // acceleration, from Newton's 2nd Law
    ax = part[i].Fx / part[i].m;
    ay = part[i].Fy / part[i].m;
    az = part[i].Fz / part[i].m;

    // Adams-Bashforth integration of velocity
    vx = part[i].vx + dt * (1.5 * ax - 0.5 * part[i].ax);
    vy = part[i].vy + dt * (1.5 * ay - 0.5 * part[i].ay);
    vz = part[i].vz + dt * (1.5 * az - 0.5 * part[i].az);

    // Adams-Bashforth integration of position
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

    // apply elastic walls (negate wall-normal velocity)
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
