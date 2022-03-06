#ifndef JLNODE_ADDON_INIT_H
#define JLNODE_ADDON_INIT_H

#include <cassert>
#include <napi.h>

namespace jlnode_addon {

Napi::BigInt Initialize(const Napi::CallbackInfo &info);

}

#endif //JLNODE_ADDON_INIT_H
