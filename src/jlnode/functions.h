#ifndef JLNODE_FUNCTIONS_H
#define JLNODE_FUNCTIONS_H

#include <napi.h>

#include "utils.h"

extern "C" {

LIBRARY_API napi_status create_function(napi_env env, void *func, const char *utf8name, napi_value *result);

}

#endif //JLNODE_FUNCTIONS_H
