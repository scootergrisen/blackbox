// -*- mode: C++; indent-tabs-mode: nil; c-basic-offset: 2; -*-
// Toolbarmenu.cc for Blackbox - an X11 Window manager
// Copyright (c) 2001 - 2003 Sean 'Shaleh' Perry <shaleh@debian.org>
// Copyright (c) 1997 - 2000, 2002 - 2003
//         Bradley T Hughes <bhughes at trolltech.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "Toolbarmenu.hh"
#include "Toolbar.hh"

class ToolbarPlacementmenu : public bt::Menu {
public:
  ToolbarPlacementmenu(bt::Application &app, unsigned int screen,
                       BScreen *bscreen);

  void refresh(void);

protected:
  void itemClicked(unsigned int id, unsigned int button);

private:
  BScreen *_bscreen;
};


enum {
  EnableToolbar,
  Placement,
  AlwaysOnTop,
  AutoHide,
  EditWorkspaceName
};


Toolbarmenu::Toolbarmenu(bt::Application &app, unsigned int screen,
                         BScreen *bscreen)
  : bt::Menu(app, screen), _bscreen(bscreen)
{
  setTitle("Toolbar Options");
  showTitle();

  ToolbarPlacementmenu *menu = new ToolbarPlacementmenu(app, screen, bscreen);
  insertItem("Enable Toolbar", EnableToolbar);
  insertSeparator();
  insertItem("Placement", menu, Placement);
  insertItem("Always on top", AlwaysOnTop);
  insertItem("Auto Hide", AutoHide);
  insertItem("Edit current workspace name", EditWorkspaceName);
}


void Toolbarmenu::refresh(void) {
  ScreenResource& res = _bscreen->resource();
  setItemChecked(EnableToolbar, res.isToolbarEnabled());
  setItemChecked(AlwaysOnTop, res.isToolbarOnTop());
  setItemChecked(AutoHide, res.doToolbarAutoHide());
}


void Toolbarmenu::itemClicked(unsigned int id, unsigned int button) {
  if (button != 1) return;

  ScreenResource& res = _bscreen->resource();
  Toolbar *toolbar = _bscreen->toolbar();

  switch (id) {
  case EnableToolbar:
    res.setToolbarEnabled(toolbar == 0);
    _bscreen->saveResource();
    if (toolbar != 0)
      _bscreen->destroyToolbar();
    else
      _bscreen->createToolbar();
    break;

  case AlwaysOnTop:
    res.saveToolbarOnTop(!res.isToolbarOnTop());
    _bscreen->saveResource();
    if (toolbar) {
      _bscreen->changeLayer(toolbar, (res.isToolbarOnTop()
                                      ? StackingList::LayerAbove
                                      : StackingList::LayerNormal));
    }
    break;

  case AutoHide:
    _bscreen->toolbar()->toggleAutoHide();
    break;

  case EditWorkspaceName:
    _bscreen->toolbar()->edit();
    break;

  default:
    break;
  } // switch
}


ToolbarPlacementmenu::ToolbarPlacementmenu(bt::Application &app,
                                           unsigned int screen,
                                           BScreen *bscreen)
  : bt::Menu(app, screen), _bscreen(bscreen)
{
  setTitle("Toolbar Placement");
  showTitle();

  insertItem("Top Left",      Toolbar::TopLeft);
  insertItem("Top Center",    Toolbar::TopCenter);
  insertItem("Top Right",     Toolbar::TopRight);
  insertSeparator();
  insertItem("Bottom Left",   Toolbar::BottomLeft);
  insertItem("Bottom Center", Toolbar::BottomCenter);
  insertItem("Bottom Right",  Toolbar::BottomRight);
}


void ToolbarPlacementmenu::refresh(void) {
  int placement = _bscreen->resource().toolbarPlacement();
  setItemChecked(Toolbar::TopLeft,      placement == Toolbar::TopLeft);
  setItemChecked(Toolbar::TopCenter,    placement == Toolbar::TopCenter);
  setItemChecked(Toolbar::TopRight,     placement == Toolbar::TopRight);
  setItemChecked(Toolbar::BottomLeft,   placement == Toolbar::BottomLeft);
  setItemChecked(Toolbar::BottomCenter, placement == Toolbar::BottomCenter);
  setItemChecked(Toolbar::BottomRight,  placement == Toolbar::BottomRight);
}


void ToolbarPlacementmenu::itemClicked(unsigned int id, unsigned int button) {
  if (button != 1) return;

  _bscreen->toolbar()->setPlacement((Toolbar::Placement) id);
}
