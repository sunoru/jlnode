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
jl_function_t *jlnode::get_reference_func = nullptr;
jl_function_t *jlnode::push_func = nullptr;

int jlnode::initialize_utils(jl_module_t *module) {
    nodecall_module = module;
    GET_FUNC_POINTER(
        push_func,
        "Base.push!",
        1
    );
    return 0;
}

jl_value_t *jlnode::to_jl_value(napi_value node_value) {
    GET_FUNC_POINTER(
        to_jl_value_func,
        "import NodeCall.napi_value_to_value;NodeCall.napi_value_to_value",
        nullptr
    );
    auto nv = jl_box_voidpointer(node_value);
    return jl_call1(to_jl_value_func, nv);
}

napi_value jlnode::to_napi_value(jl_value_t *jl_value) {
    GET_FUNC_POINTER(
        to_napi_value_func,
        "import NodeCall.napi_value_from_value;NodeCall.napi_value_from_value",
        nullptr
    );
    auto nv = jl_call1(to_napi_value_func, jl_value);
    return nv == nullptr ? nullptr : (napi_value) jl_unbox_voidpointer(nv);
}

jl_value_t *jlnode::get_reference(void *ptr) {
    GET_FUNC_POINTER(
        get_reference_func,
        "import NodeCall.jlnode_get_reference;NodeCall.jlnode_get_reference",
        nullptr
    );
    auto ref = jl_box_voidpointer(ptr);
    return jl_call1(get_reference_func, ref);
}
