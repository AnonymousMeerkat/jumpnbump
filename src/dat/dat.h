/*
 * dat.h
 * Copyright (C) 2016 "Anonymous Meerkat" <meerkatanonymous@gmail.com>
 *
 * This file is part of Jump'n'Bump.
 *
 * Jump'n'Bump is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Jump'n'Bump is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __JNB__DAT_H__
#define __JNB__DAT_H__


#include <stdint.h>


struct jnb_dat_file_entry_t
{
  char filename[12];

  char offset[4];
  char size[4];
};

struct jnb_dat_file_header_t
{
  char entries_length[4];
  struct jnb_dat_file_entry_t entries[0];
};


struct jnb_dat_entry_t;
struct jnb_dat_t
{
  int32_t entries_length;
  struct jnb_dat_entry_t* entries;

  unsigned char* data;
};

struct jnb_dat_entry_t
{
  char filename[13];
  int32_t size;

  unsigned char* contents;
};



/* Loads a .dat file into out_dat */
int jnb_dat_read(char* in_filename, struct jnb_dat_t* out_dat);


/* Finds a file entry from a .dat file */
int jnb_dat_find(struct jnb_dat_t* in_dat, char* in_filename,
                 struct jnb_dat_entry_t** out_entry);

/* Returns the contents of a file */
unsigned char* jnb_dat_open(struct jnb_dat_t* dat, char* filename);

/* Returns the size of a file */
int32_t jnb_dat_filelen(struct jnb_dat_t* dat, char* filename);


/* Destructor */
void jnb_dat_free(struct jnb_dat_t* dat);


#endif /* __JNB__DAT_H__ */
