// Rootmenu.cc for Blackbox - an X11 Window manager
// Copyright (c) 1997 - 1999 by Brad Hughes, bhughes@tcac.net
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

#ifndef   _GNU_SOURCE
#define   _GNU_SOURCE
#endif // _GNU_SOURCE

#ifdef    HAVE_CONFIG_H
#  include "../config.h"
#endif // HAVE_CONFIG_H

#include "blackbox.hh"
#include "Rootmenu.hh"
#include "Screen.hh"

#ifdef    HAVE_STDIO_H
#  include <stdio.h>
#endif // HAVE_STDIO_H

#ifdef    STDC_HEADERS
#  include <stdlib.h>
#  include <string.h>
#endif // STDC_HEADERS

#if defined(HAVE_PROCESS_H) && defined(__EMX__)
#  include <process.h>
#endif //   HAVE_PROCESS_H             __EMX__


Rootmenu::Rootmenu(Blackbox *bb, BScreen *scrn) : Basemenu(bb, scrn) {
  blackbox = bb;
  screen = scrn;
}


void Rootmenu::itemSelected(int button, int index) {
  if (button == 1) {
    BasemenuItem *item = find(index);

    if (item->function()) {
      switch (item->function()) {
      case BScreen::Execute:
	if (item->exec()) {
#ifndef   __EMX__
          int dslen =
            strlen(DisplayString(screen->getDisplay()->getDisplay()));
	  
          char *displaystring = new char[dslen + 32];
          char *command = new char[strlen(item->exec()) + dslen + 64];
	  
          sprintf(displaystring, "%s",
            DisplayString(screen->getDisplay()->getDisplay()));
          // gotta love pointer math
          sprintf(displaystring + dslen - 1, "%d", screen->getScreenNumber());
	  sprintf(command, "DISPLAY=%s exec %s &", displaystring,
		  item->exec());
	  system(command);

          delete [] displaystring;
          delete [] command;
#else // !__EMX__
	  spawnlp(P_NOWAIT, "cmd.exe", "cmd.exe", "/c", item->exec(), NULL);
#endif // __EMX__
	}
	
	break;
      	
      case BScreen::Reconfigure:
	blackbox->reconfigure();
	break;
	
      case BScreen::Restart:
	blackbox->restart();
	break;
	
      case BScreen::RestartOther:
	if (item->exec())
	  blackbox->restart(item->exec());

	break;
	
      case BScreen::Exit:
	blackbox->shutdown();
	break;

      case BScreen::SetStyle:
	if (item->exec()) {
	  blackbox->saveStyleFilename(item->exec());
	  blackbox->reconfigure();
	}
	
	break;
      }
      
      if (! (screen->getRootmenu()->hasUserMoved() || hasUserMoved()) &&
	  item->function() != BScreen::Reconfigure &&
	  item->function() != BScreen::SetStyle)
	screen->getRootmenu()->hide();
    }
  } else if (button == 3)
    screen->getRootmenu()->hide();
}

