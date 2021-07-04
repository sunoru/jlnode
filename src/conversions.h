#ifndef JLNODE_ADDON_CONVERSIONS_H
#define JLNODE_ADDON_CONVERSIONS_H

#include <string>
#include <napi.h>

#include "utils.h"
#include "instance.h"

extern "C" {

const char *to_string(napi_value value);

int get_type(napi_value value);

}

#endif //JLNODE_ADDON_CONVERSIONS_H
