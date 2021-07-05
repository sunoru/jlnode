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
    s = @ccall :libnapi_wrap.value_to_string(Env[]::NapiValue, v::NapiValue, err::Ptr{JlnodeResult})::NapiValue
    len = Ref{UInt}()
    ss = @ccall :libnapi_wrap.string_to_utf8(
        Env[]::NapiValue, s::NapiValue,
        err::Ptr{JlnodeResult}, len::Ptr{Csize_t}
    )::Ptr{Cchar}
    @show len
    ss
end
_free_string(s) = @ccall :libnapi_wrap.free_string(s::Ptr{Cchar})::Cvoid

test() = @ccall :libjlnode.test()::Cint
initialize() = begin
    ret = @ccall :libjlnode.initialize()::Cint
    Env[] = @ccall :libnapi_wrap.get_global_env()::Ptr{Cvoid}
    ret
end
dispose() = @ccall :libjlnode.dispose()::Cint

function run(scripts::AbstractString)
    result = Ref{NapiValue}()
    result = @ccall :libjlnode.run_script(Env[]::NapiValue, scripts::Cstring, err::Ptr{JlnodeResult})::NapiValue
    if err[].code == 1 || err[].code == 2
        @error unsafe_string(err[].message)
    else
        s = _to_string(result)
        t = @ccall :libnapi_wrap.value_type(
            Env[]::NapiValue, result::NapiValue, err::Ptr{JlnodeResult}
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
@show NodeCall.initialize()
@show NodeCall.Env[]

@show NodeCall.run("console.log(Math.random())")
@show NodeCall.run("Math.random()")
@show NodeCall.run("x")

if !Base.isinteractive()
    @show NodeCall.dispose()
end
