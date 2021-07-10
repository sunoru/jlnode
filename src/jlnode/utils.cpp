#include "utils.h"

jl_module_t *jlnode::nodecall_module = nullptr;
jl_function_t *jlnode::to_napi_value_func = nullptr;
jl_function_t *jlnode::to_jl_value_func = nullptr;
jl_function_t *jlnode::get_func = nullptr;
jl_function_t *jlnode::setindex_func = nullptr;
jl_function_t *jlnode::haskey_func = nullptr;
jl_function_t *jlnode::keys_func = nullptr;

int jlnode::initialize_utils(jl_module_t *module) {
    nodecall_module = module;
    get_func = jl_eval_string("get");
    haskey_func = jl_eval_string("haskey");
    keys_func = jl_eval_string("keys");
    return 0;
}

napi_value jlnode::to_napi_value(jl_value_t *jl_value) {
    if (to_napi_value_func == nullptr) {
        to_napi_value_func = jl_eval_string(
            "import NodeCall.napi_value_from_value;NodeCall.napi_value_from_value"
        );
    }
    auto nv = jl_call1(to_napi_value_func, jl_value);
    return (napi_value) jl_unbox_voidpointer(nv);
}

jl_value_t *jlnode::to_jl_value(napi_value node_value) {
    if (to_jl_value_func == nullptr) {
        to_jl_value_func = jl_eval_string(
            "import NodeCall.napi_value_to_value;NodeCall.napi_value_to_value"
        );
    }
    auto nv = jl_box_voidpointer(node_value);
    return jl_call1(to_jl_value_func, nv);
}
