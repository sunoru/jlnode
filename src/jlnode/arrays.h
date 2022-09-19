#ifndef JLNODE_ARRAYS_H
#define JLNODE_ARRAYS_H

#include <napi.h>
#include "utils.h"

extern "C" {

LIBRARY_API napi_status create_external_arraybuffer(napi_env env, void *data, size_t byte_length, napi_value *ret);

}

#endif //JLNODE_ARRAYS_H
