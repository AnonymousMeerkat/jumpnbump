/*
 * dat_read.c
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

#include "dat.h"
#include "io/io.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


#define JNB_DAT_DECODE_INT(ptr, x) ((ptr[x + 0] <<  0) +        \
                                    (ptr[x + 1] <<  8) +        \
                                    (ptr[x + 2] << 16) +        \
                                    (ptr[x + 3] << 24))

int jnb_dat_read(char* in_filename, struct jnb_dat_t* out_dat)
{
  unsigned char* read_data;
  unsigned char* ptr;
  int32_t i;
  int32_t offset;

  if (jnb_io_read(in_filename, (char**)&read_data))
    return -1;

  out_dat->data = read_data;
  out_dat->entries_length = JNB_DAT_DECODE_INT(read_data, 0);

  ptr = read_data + 4;
  out_dat->entries = malloc(sizeof(struct jnb_dat_entry_t) * out_dat->entries_length);

  if (!out_dat->entries)
    {
      perror("malloc()");
      return -2;
    }

  for (i = 0; i < out_dat->entries_length; i++)
    {
      memset(out_dat->entries[i].filename, 0, 13);
      memcpy(out_dat->entries[i].filename, ptr, 12);
      ptr += 12;

      offset = JNB_DAT_DECODE_INT(ptr, 0);
      ptr += 4;

      out_dat->entries[i].size = JNB_DAT_DECODE_INT(ptr, 0);
      ptr += 4;

      out_dat->entries[i].contents = read_data + offset;
    }

  return 0;
}


int jnb_dat_find(struct jnb_dat_t* in_dat, char* in_filename,
                 struct jnb_dat_entry_t** out_entry)
{
  int i;

  *out_entry = NULL;

  for (i = 0; i < in_dat->entries_length; i++)
    {
      if (!strnicmp(in_dat->entries[i].filename, in_filename,
                    strlen(in_filename)))
        {
          *out_entry = &in_dat->entries[i];
          break;
        }
    }

  return -!*out_entry;
}

unsigned char* jnb_dat_open(struct jnb_dat_t* dat, char* filename)
{
  struct jnb_dat_entry_t* entry;

  if (jnb_dat_find(dat, filename, &entry))
    return NULL;

  return entry->contents;
}

int32_t jnb_dat_filelen(struct jnb_dat_t* dat, char* filename)
{
  struct jnb_dat_entry_t* entry;

  if (jnb_dat_find(dat, filename, &entry))
    return -1;

  return entry->size;
}


void jnb_dat_free(struct jnb_dat_t* dat)
{
  free(dat->entries);
  free(dat->data);

  /* Mark this as freed */
  dat->entries_length = 0;
  dat->entries = NULL;
  dat->data = NULL;
}
