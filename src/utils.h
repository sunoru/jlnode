#ifndef JLNODE_ADDON_UTILS_H
#define JLNODE_ADDON_UTILS_H

#include <cstring>
#include <string>

extern "C" {

const char *copy_c_str(std::string string) {
    auto n = string.length();
    char *c_str = new char[n + 1];
    memcpy(c_str, string.data(), n);
    c_str[n] = 0;
    return c_str;
}

void free_string(const char *s) {
    delete[] s;
}

}

#endif //JLNODE_ADDON_UTILS_H
