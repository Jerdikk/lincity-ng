/*
Copyright (C) 2005 David Kamphausen <david.kamphausen@web.de>
Copyright (C) 2024 David Bears <dbear4q@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "TimerInterface.hpp"

#include <SDL.h>  // for SDL_GetTicks, Uint32

long real_time=0;
long start_time=0;

void reset_start_time()
{
  start_time=SDL_GetTicks();
}

void get_real_time()
{
  real_time=SDL_GetTicks()-start_time;
}

void get_real_time_with(Uint32 sdl_tick)
{
  real_time=sdl_tick-start_time;
}

/** @file lincity-ng/TimerInterface.cpp */
