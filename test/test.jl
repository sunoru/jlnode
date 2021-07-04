#!/bin/env julia-1.6

module NodeCall

using Libdl
const libjlnode = Libdl.dlopen(joinpath(dirname(@__DIR__), "build/lib/libjlnode"))

const NapiValue = Ptr{Nothing}

_to_string(v) = @ccall :libjlnode.to_string(v::Ptr{Cvoid})::Ptr{Cchar}
_free_string(s) = @ccall :libjlnode.free_string(s::Ptr{Cchar})::Cvoid

test() = @ccall :libjlnode.test()::Cint
initialize() = @ccall :libjlnode.initialize()::Cint
dispose() = @ccall :libjlnode.dispose()::Cint
function run(scripts::AbstractString)
    result = Ref(NapiValue())
    ret = @ccall :libjlnode.run(scripts::Cstring, result::Ptr{Ptr{Cvoid}})::Cint
    if ret == 1 || ret == 2
        @error unsafe_string(Ptr{Cchar}(result[]))
        _free_string(result[])
    else
        s = _to_string(result[])
        t = @ccall :libjlnode.get_type(result[]::NapiValue)::Cint
        @info "Type: $t"
        @info unsafe_string(s)
        _free_string(s)
    end
    ret
end

end

@show NodeCall.test()
@show NodeCall.initialize()

@show NodeCall.run("console.log(Math.random())")
@show NodeCall.run("Math.random()")
@show NodeCall.run("x")

if !Base.isinteractive()
    @show NodeCall.dispose()
end
