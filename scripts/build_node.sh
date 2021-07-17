#!/bin/sh
set -ex

cd `dirname $(realpath $0)`/../node
patch ./src/inspector_agent.cc < ../patches/inspector_agent.cc.patch
./configure --shared
make -j{nrpoc}
