#!/bin/sh
set -e

if [[ -z "$NODE_SRC" ]]; then
    echo "Should define NODE_SRC"
    exit 1
fi
cd $NODE_SRC
patch ./src/inspector_agent.cc < ../patches/inspector_agent.cc.patch
./configure --shared
make -j{nrpoc}
