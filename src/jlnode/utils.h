#ifndef JLNODE_ADDON_UTILS_H
#define JLNODE_ADDON_UTILS_H

#include <julia.h>
#include <napi.h>

namespace jlnode {

//void initialize_utils(jl_module_t *module);

napi_value to_napi_value(jl_value_t *jl_value);
jl_value_t *to_jl_value(napi_value node_value);

//extern jl_module_t *nodecall_module;

}

#endif //JLNODE_ADDON_UTILS_H
