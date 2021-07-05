#!/bin/sh
set -ex

cd `dirname $(realpath $0)`/..
./node/node ./node/deps/npm/bin/npm-cli.js run install
