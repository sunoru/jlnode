#include "utils.h"

jl_module_t *jlnode::nodecall_module = nullptr;
jl_function_t *jlnode::to_napi_value_func = nullptr;
jl_function_t *jlnode::to_jl_value_func = nullptr;
jl_function_t *jlnode::propertynames_func = nullptr;
jl_function_t *jlnode::getproperty_func = nullptr;
jl_function_t *jlnode::setproperty_func = nullptr;
jl_function_t *jlnode::hasproperty_func = nullptr;
jl_function_t *jlnode::external_finalizer_func = nullptr;
jl_function_t *jlnode::object_finalizer_func = nullptr;
jl_function_t *jlnode::arraybuffer_finalizer_func = nullptr;
jl_function_t *jlnode::call_function_func = nullptr;
jl_function_t *jlnode::get_reference_value_func = nullptr;

int jlnode::initialize_utils(jl_module_t *module) {
    nodecall_module = module;
    return 0;
}

jl_value_t *jlnode::to_jl_value(napi_value node_value) {
    GET_FUNC_POINTER(
        to_jl_value_func,
        "napi_value_to_value",
        nullptr;
    );
    auto nv = jl_box_voidpointer(node_value);
    JL_GC_PUSH1(&nv);
    auto ret = jl_call1(to_jl_value_func, nv);
    JL_GC_POP();
    return ret;
}

napi_value jlnode::to_napi_value(jl_value_t *jl_value) {
    GET_FUNC_POINTER(
        to_napi_value_func,
        "napi_value_from_value",
        nullptr;
    );
    auto nv = jl_call1(to_napi_value_func, jl_value);
    JL_GC_PUSH1(&nv);
    auto ret = nv == nullptr ? nullptr : (napi_value) jl_unbox_voidpointer(nv);
    JL_GC_POP();
    return ret;
}
