#!/bin/sh
set -ex

cd `dirname $(realpath $0)`/..
mkdir -p cmake-build
cd cmake-build
cmake ..
cmake --build .
