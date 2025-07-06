#include <utils/fileman.h>

framedir_dir dir;

#define MAX_FILES_COUNT 4096

FileManager::FileManager(IFileManager* pInterface) {
    hFilesCount = 0;
    hInterface = pInterface;
}

FileManager::~FileManager() {
    free(hFiles);
}

void FileManager::allocateFilesArray() {
    if(!hFiles)
        hFiles = (framedir_file*)
            malloc(dir.i_files * sizeof(framedir_file));
    else
        hFiles = (framedir_file*)
            realloc(hFiles, dir.i_files * sizeof(framedir_file));
}

void FileManager::freeFilesArray() {
    free(hFiles);
}

int FileManager::readCurrentDir() {
    char* cwd = (char*)malloc(FRAMEDIR_FN_MAX * sizeof(char));
    #ifdef _MSVC
        if(_getcwd(cwd, sizeof(cwd)) != NULL) {
    #elif _WATCOM
        strcpy(cwd, getcwd(NULL, 0));
        if(cwd != NULL) {
    #else
       if(getcwd(cwd, sizeof(cwd)) != NULL)
    #endif
            return readDir(cwd);
    }

    return -2;
}

int FileManager::readDir(char* pDirPath) {
    int object_index = 0;
    dir.allocated = 0;
    if(dir.i_files < 32)
        dir.i_files = 32;

    #ifdef _MSVC
        #if _MSC_VER = 1400
            sprintf_s(hCurrentPath, FRAMEDIR_FN_MAX, "%s", pDirPath);
        #else
            sprintf(hCurrentPath, "%s", pDirPath);
        #endif
    #else
        sprintf(hCurrentPath, "%s", pDirPath);
    #endif

    if(framedir_open(&dir, (const char*)pDirPath) == -1) {

        if(hInterface != NULL)
            hInterface->onError(pDirPath, 0, -1);

        return -1;
    }

    allocateFilesArray();

    for(int i = 1; i < dir.n_files; i++) {
        if(object_index >= MAX_FILES_COUNT) {
            break;
        }

        framedir_file file;

        if(framedir_readfile_n(&dir, &file, i) == -1) {
            continue;
        }

        hFiles[object_index] = file;

        object_index++;
    }

    hFilesCount = object_index;

    if(hInterface != NULL)
        hInterface->onDirectoryRead(pDirPath, hFiles);

    framedir_close(&dir);
    return 0;
}

framedir_file FileManager::getFile(int index) {
    return hFiles[index];
}

long FileManager::getFilesCount() {
    return hFilesCount;
}

char* FileManager::getRealPath(char* pDirPath) {
    int     lastPathSlash       =  0;
    int     pathSlashesCount    = -1;

    int*    pathSlashes         = (int*)malloc(192 * sizeof(int));
    int     pathLen             = strlen(pDirPath);

    for(int i = 0; i < pathLen; i++) {
        if(pDirPath[i] == '/' || pDirPath[i] == '\\') {
            pathSlashesCount++;
            pathSlashes[pathSlashesCount] = i;
        }
    }

    if(pathSlashesCount > 1) {
        if(
            pDirPath[pathLen - 3] == '/' &&
            pDirPath[pathLen - 2] == '.' &&
            pDirPath[pathLen - 1] == '.'
        ) {
            lastPathSlash = pathSlashes[pathSlashesCount - 2];
            pDirPath[lastPathSlash] = '\0';
        }
    } else if(pathSlashesCount == 1) {
        if(
            pDirPath[pathLen - 3] == '/' &&
            pDirPath[pathLen - 2] == '.' &&
            pDirPath[pathLen - 1] == '.'
        ) {
            pDirPath[lastPathSlash] = '\0';
        }
    }

    free(pathSlashes);
    return pDirPath;
}

char* FileManager::getCurrentPath() {
    return hCurrentPath;
}

