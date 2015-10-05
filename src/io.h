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

// free allocated space at the end
void clean_up(void);

#endif
