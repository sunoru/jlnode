#ifndef JLNODE_ADDON_STRINGS_H
#define JLNODE_ADDON_STRINGS_H

#include <cstring>
#include <string>

extern "C" {

const char *copy_utf8(const std::string &string, size_t *n) {
    auto m = string.length();
    *n = m;
    char *c_str = new char[m + 1];
    memcpy(c_str, string.data(), m);
    c_str[m] = 0;
    return c_str;
}

const char *copy_utf16(const std::u16string &string, size_t *n) {
    auto m = string.length();
    *n = m;
    char *c_str = new char[m * 2 + 1];
    memcpy(c_str, string.data(), m * 2);
    c_str[m] = 0;
    return c_str;
}

void free_string(const char *s) {
    delete[] s;
}

}

#endif //JLNODE_ADDON_STRINGS_H
