#!/bin/bash
cd build
# Exit on failure
set -e
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
