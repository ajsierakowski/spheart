/* Written by Adam Sierakowski 2015
 * for JHU HEART: The Computer Laboratory
 *
 * Compile with: 'make'
 */

#include "sph.h"
#include "io.h"
#include "vtk.h"

// define global variables
int np;
part_struct *part;

// begin main program
int main(void) {
  // read configuration
  read_config();

  // show configuration
  show_config();

  // initialize VTK file output
  init_vtk();

  // write VTK file output
  out_vtk(0, 0., np, part);

  // clean up
  clean_up();
 
  return EXIT_SUCCESS;
}
