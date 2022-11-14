#include "utils.h"

jl_functions jlnode::util_functions;

int jlnode::initialize_utils(jl_functions functions) {
    util_functions = functions;
    return 0;
}