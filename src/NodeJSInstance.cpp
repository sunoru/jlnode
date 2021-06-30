#include "NodeJSInstance.h"

NodeJSInstance::NodeJSInstance(void *loop) {
    if (loop == nullptr) {
        own_event_loop = true;
        event_loop = new uv_loop_t;
    } else {
        event_loop = (uv_loop_t *) loop;
    }
}

NodeJSInstance::~NodeJSInstance() {
    if (own_event_loop) {
        delete event_loop;
    }
    if (!initialized) {
        return;
    }
}

int NodeJSInstance::Initialize() {
    char pt[256];
    uv_get_process_title(pt, 256);
    std::vector<std::string> args{std::string(pt)};
    std::vector<std::string> exec_args;
    std::vector<std::string> errors;
    int exit_code = node::InitializeNodeWithArgs(&args, &exec_args, &errors);
    for (const std::string &error : errors)
        fprintf(stderr, "%s: %s\n", args[0].c_str(), error.c_str());
    if (exit_code != 0) {
        return exit_code;
    }
    return 0;
}

int NodeJSInstance::Run(const std::string scripts) {
    return 0;
}
