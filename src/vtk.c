/* Written by Adam Sierakowski 2015
 * for JHU HEART: The Computer Laboratory
 *
 * Compile with: 'make'
 */

#include "vtk.h"

// initialize output file
void init_vtk(void)
{
  // open file
  char fname[BUFLEN] = "";
  sprintf(fname, "%s/out.pvd", OUTDIR);
  FILE *ofile = fopen(fname, "w");
  if(ofile == NULL) {
    fprintf(stderr, "Could not open file %s/n", fname);
    exit(EXIT_FAILURE);
  }

  // write header and footer
  fprintf(ofile, "<VTKFile type=\"Collection\">\n");
  fprintf(ofile, "<Collection>\n");
  fprintf(ofile, "</Collection>\n");
  fprintf(ofile, "</VTKFile>");

  // close file
  fclose(ofile);
}

// write output file helper function
void out_vtk(int stepnum, float time, int n, part_struct *part)
{
  // update PVD file
  char fname_pvd[BUFLEN] = "";
  char fname_vtp[BUFLEN] = "";
  sprintf(fname_pvd, "%s/out.pvd", OUTDIR);
  sprintf(fname_vtp, "out_%d.vtp", stepnum);

  FILE *opvd = fopen(fname_pvd, "r+");
  if(opvd == NULL) {
    init_vtk();
    opvd = fopen(fname_pvd, "r+");
  }
  // move back 2 lines from the end of the file (above the footer)
  fseek(opvd, -24, SEEK_END);

  // link VTP file
  fprintf(opvd, "<DataSet timestep=\"%e\" part=\"0\" file=\"%s\"/>\n",
    time, fname_vtp);
  fprintf(opvd, "</Collection>\n");
  fprintf(opvd, "</VTKFile>");
  fclose(opvd);

  // write particle file
  part_out_vtk(stepnum, n, part);
}

// write output file
void part_out_vtk(int stepnum, int n, part_struct *part)
{
  int i;                      // iterator

  // open file
  char fname[BUFLEN] = "";
  sprintf(fname, "%s/out_%d.vtp", OUTDIR, stepnum);
  FILE *ofile = fopen(fname, "w");
  if(ofile == NULL) {
    fprintf(stderr, "Could not open file %s\n", fname);
    exit(EXIT_FAILURE);
  }

  // set up the particle output file header
  fprintf(ofile, "<VTKFile type=\"PolyData\">\n");
  fprintf(ofile, "<PolyData>\n");
  fprintf(ofile, "<Piece NumberOfPoints=\"%d\" NumberOfVerts=\"0\" ", n);
  fprintf(ofile, "NumberOfLines=\"0\" NumberOfStrips=\"0\" ");
  fprintf(ofile, "NumberOfPolys=\"0\">\n");
  fprintf(ofile, "<Points>\n");
  fprintf(ofile, "<DataArray type=\"Float32\" NumberOfComponents=\"3\" ");
  fprintf(ofile, "format=\"ascii\">\n");

  // write the locations of the particle centers
  for(i = 0; i < n; i++) {
    fprintf(ofile, "%e %e %e ", part[i].x, part[i].y, part[i].z);
  }

  fprintf(ofile, "\n</DataArray>\n");
  fprintf(ofile, "</Points>\n");

  fprintf(ofile, "<PointData Scalars=\"n m\" Vectors=\"force\">\n");

  // write particle mass
  fprintf(ofile, "<DataArray type=\"Float32\" Name=\"m\" format=\"ascii\">\n");
  for(i = 0; i < n; i++) {
    fprintf(ofile, "%e ", part[i].m);
  }
  fprintf(ofile, "\n</DataArray>\n");

  // write particle data
  fprintf(ofile, "<DataArray type=\"Float32\" NumberOfComponents=\"3\" ");
  fprintf(ofile, "Name=\"force\" format=\"ascii\">\n");
  for(i = 0; i < n; i++) {
    fprintf(ofile, "%e %e %e ", part[i].Fx, part[i].Fy, part[i].Fz);
  }
  fprintf(ofile, "\n</DataArray>\n");

  // complete the particle output footer
  fprintf(ofile, "</PointData>\n");
  fprintf(ofile, "</Piece>\n");
  fprintf(ofile, "</PolyData>\n");
  fprintf(ofile, "</VTKFile>\n");

  fclose(ofile);
}
