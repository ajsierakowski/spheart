/* Written by Adam Sierakowski 2015
 * for JHU HEART: The Computer Laboratory
 *
 * Compile with: 'make'
 */

#include "io.h"
#include "phys.h"
#include "sph.h"
#include "vtk.h"

// define global variables
int np;
part_struct *part;
float ttime;
float duration;
float dt;
int stepnum;

// begin main program
int main(int argc, char *argv[])
{
  // check command line arguments
  // if a '-s' is given, run random seeder
  if(argc == 2 && strcmp(argv[1], "-s") == 0) {
    printf("Randomly seed particles? (y/N) ");
    char buf[BUFLEN] = "";
    fgets(buf, BUFLEN, stdin);

    // continue if 'y' or 'Y' inputted
    if(strcmp(buf, "y\n") == 0 || strcmp(buf, "Y\n") == 0) {
      float xs, xe, ys, ye, zs, ze;   // domain minima and maxima

      // prompt for number of particles
      printf("Number of particles to seed: ");
      fgets(buf, BUFLEN, stdin);
      np = atoi(buf);

      // prompt for minima and maxima
      printf("Minimum in x direction: ");
      fgets(buf, BUFLEN, stdin);
      xs = atof(buf);
      printf("Maximum in x direction: ");
      fgets(buf, BUFLEN, stdin);
      xe = atof(buf);
      printf("Minimum in y direction: ");
      fgets(buf, BUFLEN, stdin);
      ys = atof(buf);
      printf("Maximum in y direction: ");
      fgets(buf, BUFLEN, stdin);
      ye = atof(buf);
      printf("Minimum in z direction: ");
      fgets(buf, BUFLEN, stdin);
      zs = atof(buf);
      printf("Maximum in z direction: ");
      fgets(buf, BUFLEN, stdin);
      ze = atof(buf);

      // write random configuration
      write_config_random(np, xs, xe, ys, ye, zs, ze);
    }
  } else if(argc > 1) {
    printf("spheart usage: '-s' to randomly seed configuration\n");
  } else { // otherwise, run simulation

    // read configuration
    read_config();

    // show configuration
    show_config();

    // initialize VTK file output
    init_vtk();

    // write initial VTK file output
    out_vtk(stepnum, ttime, np, part);

    /** do time stepping **/
    while(ttime < duration) {
      // increment total time and step number
      ttime += dt;
      stepnum++;

      printf("Computing time %f of %f (dt = %f)\n", ttime, duration, dt);

      // compute forces
      compute_forces();

      // integrate particle motion
      integrate_motion();

      // write VTK file output
      if(stepnum % 10 == 0) {
        out_vtk(stepnum, ttime, np, part);
      }
    }

    // clean up
    clean_up();
  }
 
  return EXIT_SUCCESS;
}
