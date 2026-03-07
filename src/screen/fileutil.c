#include "fileutil.h"
#include <stdio.h>
#include <ctype.h>

bool filenamesMatchIgnoreExtAndCase(const char *base, const char *filename) {
    if (base == NULL || filename == NULL) {
        return false;
    }

    int i = 0;
    // Iterate through the base name (which has no extension)
    while (base[i] != '\0') {
        // Compare current characters case-insensitively
        if (tolower((unsigned char)base[i]) != tolower((unsigned char)filename[i])) {
            return false;
        }
        i++;
    }

    // After the loop, base[i] is '\0'.
    // To match, filename must either be at its end or starting an extension.
    return (filename[i] == '\0' || filename[i] == '.');
}