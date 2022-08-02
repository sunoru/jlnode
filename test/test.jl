#!/bin/env julia-1.6


module NodeCall

const lib_path = joinpath(dirname(@__DIR__), "build/lib")

using Libdl
const libjlnode = Libdl.dlopen(joinpath(lib_path, "libjlnode"))
const libjlnode = Libdl.dlopen(joinpath(lib_path, "libjlnode"))

const NapiValue = Ptr{Nothing}
const Env = Ref(Ptr{Nothing}())

struct JlnodeResult
    code::Int32
    message::Cstring
end
const err = Ref(JlnodeResult(0, C_NULL))

function _to_string(v)
    s = @ccall :libjlnode.value_to_string(err::Ptr{JlnodeResult}, Env[]::NapiValue, v::NapiValue)::NapiValue
    len = Ref{UInt}()
    ss = @ccall :libjlnode.string_to_utf8(
        err::Ptr{JlnodeResult}, Env[]::NapiValue,
        s::NapiValue,
        len::Ptr{Csize_t}
    )::Ptr{Cchar}
    @show len
    ss
end
_free_string(s) = @ccall :libjlnode.free_string(s::Ptr{Cchar})::Cvoid

test() = @ccall :libjlnode.test()::Cint
initialize(addon_path) = begin
    ret = @ccall :libjlnode.initialize(addon_path::Cstring)::Cint
    Env[] = @ccall :libjlnode.get_global_env()::Ptr{Cvoid}
    ret
end
dispose() = @ccall :libjlnode.dispose()::Cint

function run(scripts::AbstractString)
    result = Ref{NapiValue}()
    result = @ccall :libjlnode.node_eval(err::Ptr{JlnodeResult}, Env[]::NapiValue, scripts::Cstring)::NapiValue
    if err[].code == 1 || err[].code == 2
        @error unsafe_string(err[].message)
    else
        s = _to_string(result)
        t = @ccall :libjlnode.value_type(
            err::Ptr{JlnodeResult},
            Env[]::NapiValue, result::NapiValue
        )::Cint
        if err[].code != 0
            @error unsafe_string(err[].message)
        end
        @info "Type: $t"
        @info unsafe_string(s)
        _free_string(s)
    end
    err[].code
end

end

@show NodeCall.test()
@show NodeCall.initialize(joinpath(NodeCall.lib_path, "jlnode_addon.node"))
@show NodeCall.Env[]

@show NodeCall.run("console.log(Math.random())")
@show NodeCall.run("Math.random()")
@show NodeCall.run("x")

if !Base.isinteractive()
    @show NodeCall.dispose()
end
