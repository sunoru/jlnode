#ifndef JLNODE_ADDON_ARRAYS_H
#define JLNODE_ADDON_ARRAYS_H

#include <napi.h>
#include "utils.h"

extern "C" {

napi_status create_external_arraybuffer(napi_env env, void *data, size_t byte_length, napi_value *ret);

}

#endif //JLNODE_ADDON_ARRAYS_H
