#include <framedir.h>

void _framedir_get_fileext(framedir_file* file);

int framedir_open(framedir_dir* dir, const char* path) {

    BOOL bFileFound = FALSE;
	char searchPattern[FRAMEDIR_PATH_MAX + 3];
	char* ext_pos, filename[MAX_PATH];
	int errcode, test, szFilename;

	memset(dir->path, '\0', sizeof(dir->path));
	#ifdef _MSVC2005G
		strcpy_s(dir->path, FRAMEDIR_PATH_MAX, path);
	#else
		strcpy(dir->path, path, FRAMEDIR_PATH_MAX - 1);
	#endif

	dir->_files = NULL;
	dir->n_files = 0;
	
	if (dir->i_files <= 0)
		dir->i_files = 8;

	#ifdef _MSVC2005G
		sprintf_s(
			searchPattern, 
			FRAMEDIR_PATH_MAX,
			"%s\\*.*", 
			path
		);
	#else
		snprintf(
			searchPattern,
			"%s\\*.*",
			path
		);
	#endif

		dir->_h = FindFirstFileA(searchPattern, &dir->_f);

		errcode = GetLastError();

		if(dir->_h != INVALID_HANDLE_VALUE) {

			if (dir->allocated == 0) {
				dir->_files = (framedir_file*)malloc(
					dir->i_files * sizeof(framedir_file)
				);
				dir->allocated = 1;
			}

			do {

				if (!dir->_files) break;

				// Filling file structure
				#ifdef _MSVC2005G
					strcpy_s(
						dir->_files[dir->n_files].name, 
						FRAMEDIR_FN_MAX,
						dir->_f.cFileName
					);
				#else
					strcpy(
						dir->_files[dir->n_files].name,
						dir->_f.cFileName
					);
				#endif

				szFilename = strlen(dir->_f.cFileName);
			
				#ifdef _MSVC2005G
					strcpy_s(
						dir->_files[dir->n_files].path, 
						FRAMEDIR_PATH_MAX,
						path
					);
					strcat_s(
						dir->_files[dir->n_files].path, 
						FRAMEDIR_PATH_MAX,
						"\\"
					);
				#else
					strcpy(
						dir->_files[dir->n_files].path,
						path
					);
					strcat(
						dir->_files[dir->n_files].path, "\\"
					);
				#endif
				
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

			FindClose(dir->_h);
			return 0;

		} else {
			FindClose(dir->_h);
			return -1;

		}
}

char* framedir_normalize_path(const char* path) {

    size_t len = strlen(path);
    char *n_path = (char*)malloc(FRAMEDIR_FN_MAX * sizeof(char));
	const char delimiter[] = "\\";
	char *token;
	int depth;
	char* context = NULL;
    
    if (!n_path) {
        return NULL;
    }
    
    n_path[0] = '\0';

	#ifdef _MSVC2005G
		token = strtok_s((char*)path, delimiter, &context);
	#else
		token = strtok((char*)path, delimiter);
	#endif

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
				#ifdef _MSVC2005G
					strcat_s(n_path, FRAMEDIR_FN_MAX, "\\");
				#else
					strcat(n_path, "\\");
				#endif
            }
			#ifdef _MSVC2005G
				strcat_s(n_path, FRAMEDIR_FN_MAX, token);
			#else
				strcat(n_path, token);
			#endif
            depth++;  
        }
		#ifdef _MSVC2005G
			token = strtok_s(NULL, delimiter, &context);
		#else
			token = strtok(NULL, delimiter);
		#endif
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
		#ifdef _MSVC2005G
			file->ext = _strdup(pos + 1);
		#else
			file->ext = strdup(pos + 1);
		#endif
	else
		file->ext = "";
}

void framedir_close(framedir_dir* dir) {
	if (dir == NULL)
		return;

	memset(dir->path, 0, sizeof(dir->path));
	dir->n_files = 0;
	dir->i_files = 0;
	dir->allocated = 0;
	free(dir->_files);
	dir->_files = NULL;
	#ifdef _WIN32
		if (dir->_h != INVALID_HANDLE_VALUE) {
			FindClose(dir->_h);
		}
		dir->_h = INVALID_HANDLE_VALUE;
	#endif
}