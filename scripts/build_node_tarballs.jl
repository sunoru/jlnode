# It's for libnode
using BinaryBuilder, Pkg

name = "libnode"
version = v"14.17.3"

# Collection of sources required to complete build
sources = [
    ArchiveSource("https://nodejs.org/dist/v14.17.3/node-v14.17.3.tar.gz", "dcbd156506ee79ee48439257626ca0a6db3d7eab8cb0208db6979125ae7d3a95"),
]

host_platform = HostPlatform()


# Bash recipe for building across all platforms
script = raw"""
cd $WORKSPACE/srcdir
cd node-*

export CC_host=$HOSTCC
export CXX_host=$HOSTCXX
patch ./node/src/inspector_agent.cc < ./patches/inspector_agent.cc.patch
cd node

if [[ $target == $MACHTYPE ]]
then
    ./configure --prefix=${prefix} --shared --no-cross-compiling
else
    DEST_CPU=x86_64
    if [[ $target == *i686* ]]; then DEST_CPU=x86; fi
    if [[ $target == *aarch64* ]]; then DEST_CPU=arm64; fi
    if [[ $target == *powerpc64* ]]; then DEST_CPU=ppc64; fi
    DEST_OS=linux
    if [[ $target == *apple* ]]; then DEST_OS=mac; fi
    if [[ $target == *freebsd* ]]; then DEST_OS=freebsd; fi
    if [[ $target == *w64* ]]; then DEST_OS=win; fi
    ./configure --prefix=${prefix} --shared --without-intl --cross-compiling --dest-cpu=$DEST_CPU --dest-os=$DEST_OS
fi

export LDFLAGS=-lrt
export CPPFLAGS=-D__STDC_FORMAT_MACROS
# make -j${nproc}
make -j12
make install
cp ./out/Release/node ${bindir}
cp ./LICENSE ${prefix}/
install_license ./LICENSE
"""

platforms = [
    x for x in supported_platforms()
    if BinaryBuilder.os(x) ∉ ("freebsd", "windows", "macos") &&
        nbits(x) ≡ 64 && !startswith(arch(x), "powerpc")
]

# The products that we will ensure are always built
products = [
    LibraryProduct("libnode", :libnode),
    ExecutableProduct("node", :node),
]

# Dependencies that must be installed before this package can be built
dependencies = Dependency[
]

init_block = raw"""
"""

# Build the tarballs, and possibly a `build.jl` as well.
build_tarballs(
    ARGS, name, version, sources, script,
    platforms, products, dependencies;
    preferred_gcc_version = v"10.2.0",
    init_block = init_block
)
