#!/bin/bash
cd build
# Exit on failure
set -e
cmake -DUNIT_TESTS=ON ..
make

# Run the tests
tests/CarUnitTests
