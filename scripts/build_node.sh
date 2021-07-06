#!/bin/sh
set -ex

cd `dirname $(realpath $0)`/../node
./configure --shared --without-inspector
make -j{nrpoc}
