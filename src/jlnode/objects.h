#ifndef JLNODE_ADDON_OBJECTS_H
#define JLNODE_ADDON_OBJECTS_H

#include <vector>
#include <julia.h>
#include <napi.h>

#include "utils.h"

extern "C" {

napi_value create_object_dict(napi_env env, jl_value_t *dict);
napi_value create_object_mutable(napi_env env, jl_value_t *v);

}


#endif //JLNODE_ADDON_OBJECTS_H
