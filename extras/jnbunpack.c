/*
 * unpack.c
 * Copyright (C) 1998 Brainchild Design - http://brainchilddesign.com/
 *
 * Copyright (C) 2001 "timecop" <timecop@japan.co.jp>
 *
 * Copyright (C) 2002 Florian Schulze <crow@icculus.org>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "io/io.h"
#include "dat/dat.h"

void usage(char* argv0)
{
  printf("usage: %s [-l] file.dat [-o output]\n", argv0);
}

void list_files(struct jnb_dat_t* dat)
{
  int i;
  int32_t total_size = 0;
  struct jnb_dat_entry_t entry;


  printf("  Length    Name\n---------   ----\n");


  for (i = 0; i < dat->entries_length; i++)
    {
      entry = dat->entries[i];

      total_size += entry.size;

      printf("%*d   %s\n", 9, entry.size, entry.filename);
    }


  printf("---------   -------\n%*d   %d file(s)\n", 9, total_size,
         dat->entries_length);
}

int extract_files(char* output, struct jnb_dat_t* dat)
{
  int i;
  char out_filename[4096];
  struct jnb_dat_entry_t entry;


  if (jnb_io_exists(output) != JNB_IO_EXISTS_DIR)
    if (jnb_io_mkdir(output))
      return -1;


  printf("Output directory:  %s\n", output);


  for (i = 0; i < dat->entries_length; i++)
    {
      entry = dat->entries[i];

      strcpy(out_filename, output);
      strcat(out_filename, "/");
      strcat(out_filename, entry.filename);

      printf("%*s %s\n", 11, "creating:", out_filename);

      jnb_io_write(out_filename, (char*)entry.contents, entry.size);
    }

  return 0;
}

int main(int argc, char** argv)
{
  int c;
  int list_flag = 0;
  char* file = NULL;
  char* output = ".";
  struct jnb_dat_t dat;

  /*** Parse arguments ***/

  while ((c = getopt(argc, argv, "hlo:")) != -1)
    switch (c)
      {
      case 'h':
        /* Help */
        usage(argv[0]);
        return 0;

      case 'l':
        /* List files */
        list_flag = 1;
        break;

      case 'o':
        /* Output directory */
        output = optarg;
        break;

      case '?':
        /* Unknown option */
        fprintf(stderr, "Unknown option `%c'.\n", optopt);
        usage(argv[0]);
        return -1;
      }

  if (optind < argc)
      file = argv[optind];


  /*** Run ***/

  /* Check if file is valid */
  if (!file)
    {
      usage(argv[0]);
      return 0;
    }
  else
    file = realpath(file, NULL);


  /* Print header */
  printf("Archive:  %s\n",
         file);


  /* Read */
  if (jnb_dat_read(file, &dat))
    return -2;


  /* Do the requested operation */
  if (list_flag)
    list_files(&dat);
  else
    if (extract_files(output, &dat))
      return -3;

  /* Free */
  jnb_dat_free(&dat);
  free(file);
}
