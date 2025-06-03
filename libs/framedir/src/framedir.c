#include <framedir.h>

void _framedir_get_fileext(framedir_file* file);

int framedir_open(framedir_dir* dir, const char* path) {

    BOOL bFileFound = FALSE;
	char searchPattern[FRAMEDIR_PATH_MAX + 3];
	char* ext_pos, filename[MAX_PATH];
	int errcode, test, szFilename;

	memset(dir->path, '\0', sizeof(dir->path));
    strncpy(dir->path, path, FRAMEDIR_PATH_MAX - 1);

    dir->_files = NULL;
    dir->n_files = 0;

	if (dir->i_files <= 0)
		dir->i_files = 8;

	sprintf(
		searchPattern, 
		"%s\\*.*", 
		path
	);

	dir->_h = FindFirstFileA(searchPattern, &dir->_f);

	errcode = GetLastError();

	if(dir->_h != INVALID_HANDLE_VALUE) {

		do {

			if (dir->allocated == 0) {
				dir->_files = (framedir_file*)malloc(
					dir->i_files * sizeof(framedir_file)
				);
				dir->allocated = 1;
			}
				
			if(!dir->_files) break;

			// Filling file structure

			strcpy(
				dir->_files[dir->n_files].name, 
				dir->_f.cFileName
			);

			szFilename = strlen(dir->_f.cFileName);

			strcpy(
				dir->_files[dir->n_files].path, 
				path
			);
			strcat(
				dir->_files[dir->n_files].path, "\\"
			);
				
			/* 
			 * To maintain backward compatibility with Windows NT,
			 *
			 * FILE_ATTRIBUTE_DEVICE, 
			 * FILE_ATTRIBUTE_ENCRYPTED, 
			 * FILE_ATTRIBUTE_INTEGRITY_STREAM, 
			 * FILE_ATTRIBUTE_NO_SCRUB_DAT 
			 *
			 * constants are not supported.
			 */

			dir->_files[dir->n_files].is_dir = 
				(dir->_f.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? 
				1 : 0;

			_framedir_get_fileext(&(dir->_files[dir->n_files]));

			++dir->n_files;

			if (dir->n_files == dir->i_files) {

				if (dir->i_files < 32) {
					dir->i_files *= 4;
				} if (dir->i_files < 128) {
					dir->i_files *= 2;
				} else if (dir->i_files < 512) {
					dir->i_files *= 1.5;
				} else if(dir->i_files < 4096) {
					dir->i_files *= 1.25;
				} else {
					dir->i_files *= 1.05;
				}

				dir->_files = (framedir_file*)realloc(
					dir->_files,
					dir->i_files * sizeof(framedir_file)
				);
			}

		} while(FindNextFileA(dir->_h, &dir->_f));

		return 0;

	} else {

		return -1;

	}
}

char* framedir_normalize_path(const char* path) {

    size_t len = strlen(path);
    char *n_path = (char*)malloc(FRAMEDIR_FN_MAX * sizeof(char));
	const char delimiter[] = "\\";
	char *token;
	int depth;
    
    if (!n_path) {
        return NULL;
    }
    
    n_path[0] = '\0';

    token = strtok((char*)path, delimiter);

    depth = 0;

    while (token != NULL) {
        if (strcmp(token, "..") == 0) {
            if (depth > 0) {
                char *lastSlash = strrchr(n_path, '\\');
                
                if (lastSlash != NULL) {
                    *lastSlash = '\0';
                    depth--;
                }
            }
        } else if (strcmp(token, ".") != 0) {
            if (strlen(n_path) > 0) {
                strcat(n_path, "\\");
            }
            strcat(n_path, token); 
            depth++;  
        }
        
        token = strtok(NULL, delimiter);
    }

    return n_path;
}

int framedir_readfile_n(
	const framedir_dir* dir, 
	framedir_file *file, 
	int index
) {
	if(dir == NULL || file == NULL) {
		errno = EINVAL; // invalid argument;
		return -1;
	} else if(index >= dir->n_files) {
		errno = ENOENT;
		return -1;
	}

	memcpy(file, &dir->_files[index], sizeof(framedir_file));
	_framedir_get_fileext(file);
}

void _framedir_get_fileext(framedir_file* file) {
	char* pos = strrchr(file->name, '.');
	if(pos)
		file->ext = strdup(pos + 1);
	else
		file->ext = "";
}

void framedir_close(framedir_dir* dir) {
	if (dir == NULL)
		return;

	memset(dir->path, 0, sizeof(dir->path));
	dir->n_files = 0;
	free(dir->_files);
	dir->_files = NULL;
	#ifdef _WIN32
		if (dir->_h != INVALID_HANDLE_VALUE) {
			FindClose(dir->_h);
		}
		dir->_h = INVALID_HANDLE_VALUE;
	#endif
}