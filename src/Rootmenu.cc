//
// Basemenu.cc for Blackbox - an X11 Window manager
// Copyright (c) 1997, 1998 by Brad Hughes, bhughes@tcac.net
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// (See the included file COPYING / GPL-2.0)
//

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "Rootmenu.hh"
#include "blackbox.hh"

#include <stdio.h>
#include <stdlib.h>
#ifdef __EMX__
#include <process.h>
#endif


Rootmenu::Rootmenu(Blackbox *bb) : Basemenu(bb) {
  blackbox = bb;
}


Rootmenu::~Rootmenu(void) {

}


void Rootmenu::itemSelected(int button, int index) {
  if (button == 1) {
    BasemenuItem *item = find(index);

    if (item->function()) {
      switch (item->function()) {
      case Blackbox::B_Execute:
	if (item->exec()) {
	  char *command = new char[strlen(item->exec()) + 8];
#ifndef __EMX__
	  sprintf(command, "exec %s &", item->exec());
	  system(command);
#else
	  sprintf(command, "%s", item->exec());
	  spawnlp(P_NOWAIT, "cmd.exe", "cmd.exe", "/c", command, NULL);
#endif
	  delete [] command;
	}
	
	break;
      
      case Blackbox::B_ExecReconfigure:
	if (item->exec()) {
	  char *command = new char[strlen(item->exec()) + 1];
	  sprintf(command, "%s", item->exec());
	  system(command);
	  delete [] command;
	}
	
      case Blackbox::B_Reconfigure:
	blackbox->Reconfigure();
	break;
	
      case Blackbox::B_Restart:
	blackbox->Restart();
	break;
	
      case Blackbox::B_RestartOther:
	if (item->exec())
	  blackbox->Restart(item->exec());

	break;
	
      case Blackbox::B_Exit:
	blackbox->Exit();
	break;

      case Blackbox::B_SetStyle:
	if (item->exec()) {
	  blackbox->setStyle(item->exec());
	  blackbox->Reconfigure();
	}
	
	break;
      }
      
      if (! blackbox->Menu()->userMoved() &&
	  item->function() != Blackbox::B_Reconfigure &&
	  item->function() != Blackbox::B_ExecReconfigure &&
	  item->function() != Blackbox::B_SetStyle)
	blackbox->Menu()->Hide();
    }
  } else if (button == 3)
    blackbox->Menu()->Hide();
}
