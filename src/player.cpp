/*
 * Adplug - Replayer for many OPL2/OPL3 audio file formats.
 * Copyright (C) 1999 - 2002 Simon Peter, <dn.tlp@gmx.net>, et al.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * player.cpp - Replayer base class, by Simon Peter <dn.tlp@gmx.net>
 */

#include <string.h>

#include "player.h"
#include "adplug.h"

/***** CPlayer *****/

const unsigned short CPlayer::note_table[12] =
  {363, 385, 408, 432, 458, 485, 514, 544, 577, 611, 647, 686};

const unsigned char CPlayer::op_table[9] =
  {0x00, 0x01, 0x02, 0x08, 0x09, 0x0a, 0x10, 0x11, 0x12};

CPlayer::CPlayer(Copl *newopl)
  : opl(newopl), db(CAdPlug::database)
{
}

CPlayer::~CPlayer()
{
}

bool CPlayer::extension(const std::string &filename, const std::string &extension)
{
  const char *fname = filename.c_str(), *ext = extension.c_str();

  if(strlen(fname) < strlen(ext) ||
     stricmp(fname + strlen(fname) - 4, ext))
    return false;
  else
    return true;
}

unsigned long CPlayer::filesize(binistream *f)
{
  unsigned long oldpos = f->pos(), size;

  f->seek(0, binio::End);
  size = f->pos();
  f->seek(oldpos, binio::Set);

  return size;
}

unsigned long CPlayer::songlength(int subsong)
{
  float slength = 0.0f;

  // get song length
  rewind(subsong);
  while(update() && slength < 600000)	// song length limit: 10 minutes
    slength += 1000/getrefresh();
  rewind(subsong);

  return (unsigned long)slength;
}

void CPlayer::seek(unsigned long ms)
{
  float pos = 0.0f;

  rewind();
  while(pos < ms && update())		// seek to new position
    pos += 1000/getrefresh();
}