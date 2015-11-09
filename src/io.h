/* Written by Adam Sierakowski 2015
 * for JHU HEART: The Computer Laboratory
 *
 * Compile with: 'make'
 */

#ifndef _IO_H
#define _IO_H

// read simulation configuration
void read_config(void);

// write to STDOUT simulation configuration
void show_config(void);

// write random simulation configuration
void write_config_random(int np, float r, float m, float E,
  float xs, float xe, float ys, float ye, float zs, float ze);

// free allocated space at the end
void clean_up(void);

#endif
