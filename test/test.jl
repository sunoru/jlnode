#!/bin/env julia-1.6

module NodeCall

using Libdl
# const libnapi_wrap = Libdl.dlopen(joinpath(dirname(@__DIR__), "build/lib/libnapi_wrap"))
const libjlnode = Libdl.dlopen(joinpath(dirname(@__DIR__), "build/lib/libjlnode"))

const NapiValue = Ptr{Nothing}

function _to_string(v)
    s = @ccall :libnapi_wrap.value_to_string(v::NapiValue)::NapiValue
    @ccall :libnapi_wrap.string_to_utf8(s::NapiValue)::Ptr{Cchar}
end
_free_string(s) = @ccall :libnapi_wrap.free_string(s::Ptr{Cchar})::Cvoid

test() = @ccall :libjlnode.test()::Cint
initialize() = @ccall :libjlnode.initialize()::Cint
open_handle_scope() = @ccall :libnapi_wrap.open_handle_scope()::Ptr{Cvoid}
close_handle_scope() = @ccall :libnapi_wrap.close_handle_scope()::Cvoid
dispose() = @ccall :libjlnode.dispose()::Cint
function run(scripts::AbstractString)
    result = Ref(NapiValue())
    ret = @ccall :libjlnode.run(scripts::Cstring, result::Ptr{Ptr{Cvoid}})::Cint
    if ret == 1 || ret == 2
        @error unsafe_string(Ptr{Cchar}(result[]))
        _free_string(result[])
    else
        s = _to_string(result[])
        t = @ccall :libnapi_wrap.value_type(result[]::NapiValue)::Cint
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
