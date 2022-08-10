#include "utils.h"

jl_module_t *jlnode::nodecall_module = nullptr;
jl_function_t *jlnode::propertynames_func = nullptr;
jl_function_t *jlnode::getproperty_func = nullptr;
jl_function_t *jlnode::setproperty_func = nullptr;
jl_function_t *jlnode::hasproperty_func = nullptr;
jl_function_t *jlnode::external_finalizer_func = nullptr;
jl_function_t *jlnode::object_finalizer_func = nullptr;
jl_function_t *jlnode::arraybuffer_finalizer_func = nullptr;
jl_function_t *jlnode::call_function_func = nullptr;
jl_function_t *jlnode::call_threadsafe_func = nullptr;

int jlnode::initialize_utils(jl_module_t *module) {
    nodecall_module = module;
    return 0;
}