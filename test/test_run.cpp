#include <string>

#include "../src/NodeJSInstance.h"

int main(int argc, char **argv) {
    uv_loop_t loop;
    NodeJSInstance instance(&loop);

    uv_setup_args(argc, argv);
    std::string scripts("console.log(Math.random())");

    instance.Initialize();
    instance.Run(scripts);

    return 0;
}