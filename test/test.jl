#!/bin/env julia-1.6

module NodeCall

using Libdl
const libjlnode = Libdl.dlopen(joinpath(dirname(@__DIR__), "build/lib/libjlnode"))

test() = @ccall :libjlnode.test()::Cint
initialize() = @ccall :libjlnode.initialize()::Cint
dispose() = @ccall :libjlnode.dispose()::Cint
run(scripts::AbstractString) = @ccall :libjlnode.run(scripts::Cstring)::Cint

end

@show NodeCall.test()
@show NodeCall.initialize()

@show NodeCall.run("console.log(Math.random())")
@show NodeCall.run("console.log(Math.random())")

if !Base.isinteractive()
    @show NodeCall.dispose()
end
