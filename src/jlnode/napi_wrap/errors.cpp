#include "errors.h"

JlnodeResult::JlnodeResult(int _code, const char *_message) : code(_code), message(_message) {}

JlnodeResult JlnodeResult::From(const Napi::Error &err) {
    size_t n;
    return {1, copy_utf8(err.what(), &n)};
}

JlnodeResult JlnodeResult::From(const std::exception &err) {
    size_t n;
    return {2, copy_utf8(err.what(), &n)};
}

JlnodeResult JlnodeResult::Ok() {
    return {0, nullptr};
}
