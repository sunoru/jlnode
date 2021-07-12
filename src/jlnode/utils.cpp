#include "utils.h"

jl_module_t *jlnode::nodecall_module = nullptr;
jl_function_t *jlnode::to_napi_value_func = nullptr;
jl_function_t *jlnode::to_jl_value_func = nullptr;
jl_function_t *jlnode::get_func = nullptr;
jl_function_t *jlnode::setindex_func = nullptr;
jl_function_t *jlnode::haskey_func = nullptr;
jl_function_t *jlnode::keys_func = nullptr;
jl_function_t *jlnode::propertynames_func = nullptr;
jl_function_t *jlnode::getproperty_func = nullptr;
jl_function_t *jlnode::setproperty_func = nullptr;

int jlnode::initialize_utils(jl_module_t *module) {
    nodecall_module = module;
    GET_FUNC_POINTER(get_func, "get", 1);
    GET_FUNC_POINTER(haskey_func, "haskey", 1);
    GET_FUNC_POINTER(keys_func, "keys", 1);
    GET_FUNC_POINTER(propertynames_func, "propertynames", 1);
    return 0;
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

napi_value jlnode::to_napi_value(jl_value_t *jl_value) {
    if (to_napi_value_func == nullptr) {
        to_napi_value_func = jl_eval_string(
            "import NodeCall.napi_value_from_value;NodeCall.napi_value_from_value"
        );
    }
    auto nv = jl_call1(to_napi_value_func, jl_value);
    return nv == nullptr ? nullptr : (napi_value) jl_unbox_voidpointer(nv);
}
