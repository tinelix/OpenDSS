/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -----------------------------------------------------------------------
 *  Copyright � 2024 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This file is part of Tinelix OpenDSS.
 *
 *  Tinelix OpenDSS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Tinelix OpenDSS is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Tinelix OpenDSS. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <tinydir/tinydir.h>

class IFileManager {
public:
    IFileManager() {};
    virtual ~IFileManager() {};
    virtual void onError(int cmdId, int errorCode) = 0;
    virtual void onResult(int cmdId, int resultCode) = 0;
    virtual void onDirectoryRead(tinydir_file* files) = 0;
};
