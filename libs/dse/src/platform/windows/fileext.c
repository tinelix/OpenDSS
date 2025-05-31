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
    strcpy(local_copy, extensions_list);  // Создаем копию строки, поскольку strtok модифицирует входящую строку

    char* token = strtok(local_copy, "|");
    while (token != NULL) {
        if (strcmp(token, ext) == 0) {  // Сравниваем каждое расширение с указанным
            return 1;                  // Нашли совпадение!
        }
        token = strtok(NULL, "|");      // Переходим к следующему элементу
    }
    return 0;
}