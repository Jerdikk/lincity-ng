/*
Copyright (C) 2005 Matthias Braun <matze@braunis.de>
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
#include "PhysfsSDL.hpp"

#include <SDL.h>     // for Sint64
#include <assert.h>         // for assert
#include <physfs.h>         // for PHYSFS_file, PHYSFS_seek, PHYSFS_fileLength
#include <stdio.h>          // for size_t, NULL, SEEK_CUR, SEEK_END, SEEK_SET
#include <iostream>         // for basic_ostream, operator<<, cerr, stringst...
#include <sstream>          // for basic_stringstream
#include <stdexcept>        // for runtime_error

#include "PhysfsError.hpp"  // for getPhysfsLastError

static Sint64 funcSize(struct SDL_RWops* context)
{
    PHYSFS_file* file = (PHYSFS_file*) context->hidden.unknown.data1;
    return PHYSFS_fileLength(file);
}

static Sint64 funcSeek(struct SDL_RWops* context, Sint64 offset, int whence)
{
    PHYSFS_file* file = (PHYSFS_file*) context->hidden.unknown.data1;
    size_t pos;
    switch(whence) {
        case SEEK_SET:
            pos = offset;
            break;
        case SEEK_CUR:
            pos = PHYSFS_tell(file) + offset;
            break;
        case SEEK_END:
            pos = PHYSFS_fileLength(file) + offset;
            break;
        default:
            pos = 0;
            assert(false);
            break;
    }
    int res = PHYSFS_seek(file, pos);

    if(res == 0) {
        std::cerr << "Error seeking in file: " << getPhysfsLastError() << "\n";
        return -1;
    }

    return pos;
}

static size_t funcRead(struct SDL_RWops* context, void* ptr, size_t size, size_t maxnum)
{
    PHYSFS_file* file = (PHYSFS_file*) context->hidden.unknown.data1;

    PHYSFS_sint64 res = PHYSFS_readBytes(file, ptr, size * maxnum);
    if(res < 0) {
        std::cerr << "Read error: " << getPhysfsLastError() << std::endl;
        return 0;
    }
    else if(res % size != 0) {
        std::cerr << "Warning: partial object read: " << getPhysfsLastError() << std::endl;
    }
    else if(res / size != maxnum) {
        std::cerr << "Warning: partial read: " << getPhysfsLastError() << std::endl;
    }
    return res / size;
}

static int funcClose(struct SDL_RWops* context)
{
    PHYSFS_file* file = (PHYSFS_file*) context->hidden.unknown.data1;

    PHYSFS_close(file);
    delete context;

    return 0;
}

SDL_RWops* getPhysfsSDLRWops(const std::string& filename)
{
    PHYSFS_file* file = (PHYSFS_file*) PHYSFS_openRead(filename.c_str());
    if(!file) {
        std::stringstream msg;
        msg << "Couldn't open '" << filename << "': "
            << getPhysfsLastError();
        throw std::runtime_error(msg.str());
    }

    SDL_RWops* ops = new SDL_RWops();
    ops->type = 0;
    ops->hidden.unknown.data1 = file;
    ops->size = funcSize;
    ops->seek = funcSeek;
    ops->read = funcRead;
    ops->write = NULL;
    ops->close = funcClose;
    return ops;
}

/** @file PhysfsStream/PhysfsSDL.cpp */
