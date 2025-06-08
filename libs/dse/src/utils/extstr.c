#include <utils/extstr.h>

/* String extensions for C.
 *
 * Use this code for your program.
 *
 * Author: Dmitry Tretyakov (tretdm), 2024-10-31
 */

int strcut(char* str, size_t begin, size_t len)
{
    size_t l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

    return len;
}

int strendq(const char* str, const char* suffix)
{
	size_t lenstr;
	size_t lensuffix;
    if (!str || !suffix)
        return 0;
    lenstr = strlen(str);
    lensuffix = strlen(suffix);
    if (lensuffix > lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

char* strwrap(char* out, int width) {

    char* str = (char*)malloc(sizeof(out) + 4);

    int len = 180, n, w, wordlen = 0, linepos = 0, outlen = 0;

	#ifdef _MSVC
		char* word;
    #else
		char word[len];
    #endif

    for (len = 0; len; len++) {

		#ifdef _MSVC
			word = malloc(sizeof(char) * len);
		#endif

        for (n = 0; n <= len; n++) {
            if (str[n] == ' ' || str[n] == '\n' || n == len) {
                if (linepos > width) {
                    out[outlen++] = '\n';
                    linepos = wordlen;
                }

                for (w = 0; w < wordlen; w++) {
                    out[outlen++] = word[w];
                    word[w] = '\0';
                }

                if (n == len)
                    out[outlen] = '\0';
                else if (str[n] == '\n') {
                    out[outlen] = str[n];
                    linepos = 0;
                }
                else {
                    out[outlen] = ' ';
                    linepos++;
                }

                outlen++;
                wordlen = 0;
            }
            else {
                word[wordlen++] = str[n];
                linepos++;
            }
        }
    }

    return str;
}

int strcrlfc(char* out) {
	int i;
    for (i = 0; i < (int)strlen(out); i++) {
        if (out[i] == '\r' && out[i + 1] == '\n') {
            return out[i] == '\r' && out[i + 1] == '\n';
        }
        else if (out[i] == '\n') {
            return 1;
        }
        else {
            continue;
        }
    }

    return 0;
}

size_t strlines(char* out, int useCrLf) {
    int lines = 0;
	int i;
    for (i = 0; i < (int)strlen(out); i++) {
        if (useCrLf && out[i] == '\r' && out[i + 1] == '\n') {
            lines++;
        }
        else if (out[i] == '\n') {
            lines++;
        }
    }

    return lines - 2;
}

char** strsplitln(char* out) {
    int useCrlf = strcrlfc(out);
    char** str = (char**)malloc(sizeof(char) * 1024 * strlines(out, useCrlf));
    char* stk = NULL;
	int lines;

    if (useCrlf == 0) {
        #ifdef _MSVC2005G
            stk = strtok_s(out, "\r\n", "");
        #else
            stk = strtok(out, "\r\n");
        #endif
    }
    else {
        #ifdef _MSVC2005G
        stk = strtok_s(out, "\n", "");
        #else
            stk = strtok(out, "\n");
        #endif
    }
    lines = 0;

    while (stk != NULL) {
        str[lines] = stk;
        if (useCrlf) {
            #ifdef _MSVC2005G
            stk = strtok_s(NULL, "\r\n", "");
            #else
                stk = strtok(NULL, "\r\n");
            #endif
        }
        else {
            #ifdef _MSVC2005G
                stk = strtok_s(NULL, "\n", "");
            #else
                stk = strtok(NULL, "\n");
            #endif
        }
        lines++;
    }

    return str;
}
