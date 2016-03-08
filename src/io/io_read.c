/*
 * io_read.c
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

#include "io.h"
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef BZLIB_SUPPORT
#  include <bzlib.h>
#endif

#ifdef ZLIB_SUPPORT
#  include <zlib.h>
#endif


#ifndef _MSC_VER
#include <sys/stat.h>

int filelength(int handle)
{
  struct stat buf;

  if (fstat(handle, &buf) == -1)
    {
      perror("filelength");
      return -1;
    }

  return buf.st_size;
}
#endif


int jnb_io_exists(char* filename)
{
  struct stat s;

  if (stat(filename, &s))
    return -1;

  switch (s.st_mode & S_IFMT)
    {
    case S_IFREG:
      return JNB_IO_EXISTS_FILE;
    case S_IFDIR:
      return JNB_IO_EXISTS_DIR;
    default:
      return JNB_IO_EXISTS_OTHER;
    }
}


int jnb_io_read_bz2(char* in_filename, char** out_data)
{
#ifdef BZLIB_SUPPORT

  char* bzfilename;
  BZFILE* bzf;

  bzfilename = malloc(strlen(in_filename) + 5);
  strcpy(bzfilename, in_filename);
  strcat(bzfilename, ".bz2");

  bzf = BZ2_bzopen(bzfilename, "rb");

  free(bzfilename);
  bzfilename = NULL;

  *out_data = NULL;

  if (bzf != NULL)
    {
      int bufsize = 0;
      int bufpos = 0;
      int br;
      char* ptr;

      do
        {
          if (bufpos >= bufsize)
            {
              bufsize += 1024 * 1024;
              *out_data = (char*) realloc(*out_data, bufsize);
              if (*out_data == NULL)
                {
                  perror("realloc()");
                  return -2;
                }
            }

          br = BZ2_bzread(bzf, *out_data + bufpos, bufsize - bufpos);
          if (br == -1)
            {
              fprintf(stderr, "bzread failed.\n");
              return -3;
            }

          bufpos += br;
        } while (br>0);

      /* try to shrink buffer... */
      ptr = (char*) realloc(*out_data, bufpos);
      if (ptr != NULL)
        *out_data = ptr;

      BZ2_bzclose(bzf);

      return 0;
    }

#endif /* BZLIB_SUPPORT */

  return -1;
}


int jnb_io_read_gz(char* in_filename, char** out_data)
{
#ifdef ZLIB_SUPPORT

  char* gzfilename;
  gzFile gzf;

  gzfilename = malloc(strlen(in_filename) + 4);
  strcpy(gzfilename, in_filename);
  strcat(gzfilename, ".gz");

  gzf = gzopen(gzfilename, "rb");

  free(gzfilename);
  gzfilename = NULL;

  *out_data = NULL;

  if (gzf != NULL)
    {
      int bufsize = 0;
      int bufpos = 0;
      char* ptr;
      int br;

      do
        {
          if (bufpos >= bufsize)
            {
              bufsize += 1024 * 1024;
              *out_data = (char*) realloc(*out_data, bufsize);
              if (*out_data == NULL)
                {
                  perror("realloc()");
                  return -1;
                }
            }

          br = gzread(gzf, *out_data + bufpos, bufsize - bufpos);
          if (br == -1)
            {
              fprintf(stderr, "gzread failed.\n");
              return -2;
            }

          bufpos += br;
        } while (!gzeof(gzf));

      /* try to shrink buffer... */
      ptr = (char*) realloc(*out_data, bufpos);
      if (ptr != NULL)
        *out_data = ptr;

      gzclose(gzf);

      return 0;
    }

#endif /* ZLIB_SUPPORT */

  return -1;
}


int jnb_io_read_raw(char* in_filename, char** out_data)
{
  int fd, len;

  fd = open(in_filename, O_RDONLY | O_BINARY);

  if (fd == -1)
    {
      fprintf(stderr, "can't open %s: ", in_filename);
      perror("");
      return -1;
    }

  len = filelength(fd);
  if (len < 0)
    {
      close(fd);
      return -2;
    }

  *out_data = (unsigned char *) malloc(len);
  if (*out_data == NULL)
    {
      perror("malloc()");
      close(fd);
      return -3;
    }

  if (read(fd, *out_data, len) != len)
    {
      perror("read()");
      close(fd);
      return -4;
    }

  close(fd);

  return 0;
}


int jnb_io_read(char* in_filename, char** out_data)
{
  if (!jnb_io_read_bz2(in_filename, out_data))
    return 0;

  if (!jnb_io_read_gz(in_filename, out_data))
    return 0;

  return jnb_io_read_raw(in_filename, out_data);
}
