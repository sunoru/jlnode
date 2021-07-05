#ifndef JLNODE_NAPI_WRAP_ERRORS_H
#define JLNODE_NAPI_WRAP_ERRORS_H

#include <exception>
#include <napi.h>

#include "utils/strings.h"

extern "C" {

struct JlnodeResult {
    int code;
    const char *message;

    JlnodeResult(int _code, const char *_message);

    static JlnodeResult Ok();

    static JlnodeResult From(const Napi::Error &err);

    static JlnodeResult From(const std::exception &err);
};

}

#endif //JLNODE_NAPI_WRAP_ERRORS_H
