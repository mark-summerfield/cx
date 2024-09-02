// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "fx.h"
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

bool file_or_folder_exists(const char* path) {
    if (!access(path, F_OK))
        return true; // its a file
    else {
        DIR* dir = opendir(path);
        if (dir) {
            closedir(dir);
            return true; // its a folder
        }
    }
    return false;
}
