#ifndef JLNODE_NODEJSINSTANCE_H
#define JLNODE_NODEJSINSTANCE_H

#include <node.h>
#include <uv.h>

class NodeJSInstance {
public:
    uv_loop_t *event_loop;

    NodeJSInstance(void *loop);
    int Initialize();
    int Run(const std::string scripts);
    ~NodeJSInstance();

private:
    bool initialized;
    bool own_event_loop;
};


#endif //JLNODE_NODEJSINSTANCE_H
