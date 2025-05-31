#include <utils/fileext.h>
#include <stddef.h>

char* getfext(const char* filename) {
    size_t len = strlen(filename);
    for (int i = len - 1; i >= 0; i--) {
        if (filename[i] == '.') {
            char* extension = strdup(filename + i + 1);
            return extension;
        }
    }
    return NULL;
}

int chkfext(const char* extensions_list, const char* ext) {
    char local_copy[255];
    strcpy(local_copy, extensions_list);  // ������� ����� ������, ��������� strtok ������������ �������� ������

    char* token = strtok(local_copy, "|");
    while (token != NULL) {
        if (strcmp(token, ext) == 0) {  // ���������� ������ ���������� � ���������
            return 1;                  // ����� ����������!
        }
        token = strtok(NULL, "|");      // ��������� � ���������� ��������
    }
    return 0;
}