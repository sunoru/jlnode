#!/bin/sh
set -ex

cd `dirname $(realpath $0)`/../node
mkdir -p cmake-build
cd cmake-build
cmake ..
cmake --build .
