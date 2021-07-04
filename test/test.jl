#!/bin/env julia-1.6

module NodeCall
using CxxWrap
using Libdl
Libdl.dlopen("./node/out/Debug/libnode.so.83")
@wrapmodule(joinpath(dirname(@__DIR__), "build/lib/libjlnode"))
# @wrapmodule(joinpath(dirname(@__DIR__), "build/lib/jlnode.node"))
function __init__()
@initcxx
end
end

@show NodeCall.test()
@show instance = NodeCall.NodeJsInstance()
@show NodeCall.initialize(instance)
@show NodeCall.dispose(instance)
