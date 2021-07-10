#include "utils.h"

jl_module_t *jlnode::nodecall_module = nullptr;
jl_function_t *jlnode::to_napi_value_func = nullptr;
jl_function_t *jlnode::to_jl_value_func = nullptr;

int jlnode::initialize_utils(jl_module_t *module) {
    nodecall_module = module;
    return 0;
}

napi_value jlnode::to_napi_value(jl_value_t *jl_value) {
    if (to_napi_value_func == nullptr) {
        to_napi_value_func = jl_eval_string("import NodeCall.napi_value;NodeCall.napi_value");
    }
    auto nv = jl_call1(to_napi_value_func, jl_value);
    return (napi_value) jl_unbox_voidpointer(nv);
}

jl_value_t *jlnode::to_jl_value(napi_value node_value) {
    if (to_jl_value_func == nullptr) {
        to_jl_value_func = jl_eval_string("import NodeCall.value;NodeCall.value");
    }
    auto nv = jl_box_voidpointer(node_value);
    return jl_call1(to_jl_value_func, nv);
}
