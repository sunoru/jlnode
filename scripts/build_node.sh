#!/bin/sh
set -ex

cd `dirname $(realpath $0)`/../node
./configure --shared
make -j6
