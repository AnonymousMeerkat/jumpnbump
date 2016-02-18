/*
 * io.h
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

#ifndef __JNB__IO_H__
#define __JNB__IO_H__


#ifndef _MSC_VER
int filelength(int handle);
#endif

int jnb_io_read(char* in_filename, char** out_data);
int jnb_io_write(char* filename, char* data);


#endif
