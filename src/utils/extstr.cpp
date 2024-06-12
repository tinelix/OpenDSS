#include "extstr.h"

/* String extensions for C++98/C++03.
 *
 * Use this code for your program.
 *
 * Author: Dmitry Tretyakov (tretdm), 2024-05-20
 */

int ExtString::strcut(char* str, int begin, int len)
{
    int l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

    return len;
}

int ExtString::strendq(const char* str, const char* suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

char* ExtString::strwrap(char* out, int width) {

    char* str = (char*)malloc(sizeof(out) + 4);

    int len, n, w, wordlen = 0, linepos = 0, outlen = 0;

    for (len = 0; str[len]; len++) {
        #ifdef _MSVC
            char* word = new char[len];
        #else
            char word[len];
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

bool ExtString::strcrlfc(char* out) {
    for (int i = 0; i < (int)strlen(out); i++) {
        if (out[i] == '\r' && out[i + 1] == '\n') {
            return out[i] == '\r' && out[i + 1] == '\n';
        }
        else if (out[i] == '\n') {
            return false;
        }
        else {
            continue;
        }
    }

    return false;
}

size_t ExtString::strlines(char* out, bool useCrLf) {
    int lines = 0;
    for (int i = 0; i < (int)strlen(out); i++) {
        if (useCrLf && out[i] == '\r' && out[i + 1] == '\n') {
            lines++;
        }
        else if (out[i] == '\n') {
            lines++;
        }
    }

    return lines - 2;
}

char** ExtString::strsplitln(char* out) {
    bool useCrlf = strcrlfc(out);
    char** str = (char**)malloc(sizeof(char) * 1024 * strlines(out, useCrlf));

    char* stk = NULL;
    if (useCrlf) {
        #ifdef _MSVC2005G
            stk = strtok_s(out, "\r\n", NULL);
        #else
            stk = strtok(out, "\r\n");
        #endif
    }
    else {
        #ifdef _MSVC2005G
            stk = strtok_s(out, "\n", NULL);
        #else
            stk = strtok(out, "\n");
        #endif
    }
    int lines = 0;

    while (stk != NULL) {
        str[lines] = (char*)malloc(sizeof(char) * strlen(stk));
        str[lines] = stk;
        if (useCrlf) {
            #ifdef _MSVC2005G
                stk = strtok_s(NULL, "\r\n", NULL);
            #else
                stk = strtok(NULL, "\r\n");
            #endif
        }
        else {
            #ifdef _MSVC2005G
                stk = strtok_s(NULL, "\n", NULL);
            #else
                stk = strtok(NULL, "\n");
            #endif
        }
        lines++;
    }

    return str;
}
