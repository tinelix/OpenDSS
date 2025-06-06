/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -----------------------------------------------------------------------------
 *  Copyright � 2024, 2025 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License 3 (or any later version)
 *  and/or Apache License 2.
 *
 *  See the following files in repository directory for the precise terms and
 *  conditions of either license:
 *
 *     LICENSE.APACHE
 *     LICENSE.GPLv3
 *
 *  Please see each file in the implementation for copyright and licensing
 *  information (in the opening comment of each file).
 */

#pragma once

#include <framedir/include/framedir.h>

class IFileManager {
public:
    IFileManager() {};
    virtual ~IFileManager() {};
    virtual void onError(int cmdId, int errorCode) = 0;
    virtual void onResult(int cmdId, int resultCode) = 0;
    virtual void onDirectoryRead(framedir_file* files) = 0;
};
