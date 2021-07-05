#!/bin/env julia-1.6

module NodeCall

using Libdl
# const libnapi_wrap = Libdl.dlopen(joinpath(dirname(@__DIR__), "build/lib/libnapi_wrap"))
const libjlnode = Libdl.dlopen(joinpath(dirname(@__DIR__), "build/lib/libjlnode"))

const NapiValue = Ptr{Nothing}
const Env = Ref(Ptr{Nothing}())

struct JlnodeResult
    code::Int32
    message::Cstring
end
const err = Ref(JlnodeResult(0, C_NULL))

function _to_string(v)
    s = @ccall :libnapi_wrap.value_to_string(err::Ptr{JlnodeResult}, Env[]::NapiValue, v::NapiValue)::NapiValue
    len = Ref{UInt}()
    ss = @ccall :libnapi_wrap.string_to_utf8(
        err::Ptr{JlnodeResult}, Env[]::NapiValue,
        s::NapiValue,
        len::Ptr{Csize_t}
    )::Ptr{Cchar}
    @show len
    ss
end
_free_string(s) = @ccall :libnapi_wrap.free_string(s::Ptr{Cchar})::Cvoid

test() = @ccall :libjlnode.test()::Cint
initialize(addon_path) = begin
    ret = @ccall :libjlnode.initialize(addon_path::Cstring)::Cint
    Env[] = @ccall :libnapi_wrap.get_global_env()::Ptr{Cvoid}
    ret
end
dispose() = @ccall :libjlnode.dispose()::Cint

function run(scripts::AbstractString)
    result = Ref{NapiValue}()
    result = @ccall :libjlnode.run_script(err::Ptr{JlnodeResult}, Env[]::NapiValue, scripts::Cstring)::NapiValue
    if err[].code == 1 || err[].code == 2
        @error unsafe_string(err[].message)
    else
        s = _to_string(result)
        t = @ccall :libnapi_wrap.value_type(
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
@show NodeCall.initialize(joinpath(@__DIR__, "../build/lib/jlnode_addon.node"))
@show NodeCall.Env[]

@show NodeCall.run("console.log(Math.random())")
@show NodeCall.run("Math.random()")
@show NodeCall.run("x")

if !Base.isinteractive()
    @show NodeCall.dispose()
end
