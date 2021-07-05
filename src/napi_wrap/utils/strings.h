#ifndef JLNODE_NAPI_WRAP_UTILS_STRINGS_H
#define JLNODE_NAPI_WRAP_UTILS_STRINGS_H

#include <cstring>
#include <string>

extern "C" {

const char *copy_utf8(const std::string &string, size_t *n);

const char *copy_utf16(const std::u16string &string, size_t *n);

void free_string(const char *s);

}

#endif //JLNODE_NAPI_WRAP_UTILS_STRINGS_H
