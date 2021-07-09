#include "utils.h"

//jl_module_t *jlnode::nodecall_module = nullptr;
//
//napi_value jlnode::to_napi_value(jl_value_t *jl_value) {
//    auto func = jl_get_function(nodecall_module, "napi_value");
//    auto v = jl_call1(func, jl_value);
//    fprintf(stderr, "0x%lx\n", (uint64_t) v);
//    auto ret = (napi_value) jl_unbox_uint64(v);
//    fprintf(stderr, "0x%lx\n", (uint64_t) ret);
//    return ret;
//}
//
//jl_value_t *jlnode::to_jl_value(napi_value node_value) {
//    auto func = jl_get_function(nodecall_module, "value");
//    return jl_call1(func, jl_box_uint64((uint64_t) node_value));
//}
//
//void jlnode::initialize_utils(jl_module_t *module) {
//    nodecall_module = module;
//}

napi_value jlnode::to_napi_value(jl_value_t *jl_value) {
    return nullptr;
}

jl_value_t *jlnode::to_jl_value(napi_value node_value) {
    return nullptr;
}
