//  $Id$
//
//  SuperTux -  A Jump'n Run
//  Copyright (C) 2004 Matthias Braun <matze@braunis.de
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#ifndef __GLUTIL_HPP__
#define __GLUTIL_HPP__

#include <sstream>
#include <stdexcept>
#include <GL/gl.h>
#include <GL/glu.h>

static inline void assert_gl(const char* message)
{
#ifdef DEBUG
  GLenum error = glGetError();
  if(error != GL_NO_ERROR) {
    std::ostringstream msg;
    msg << "OpenGLError while '" << message << "': "
        << gluErrorString(error);
    throw std::runtime_error(msg.str());
  }
#endif
}

#endif
