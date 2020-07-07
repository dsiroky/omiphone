#!/bin/bash
set -e

for debug in 0 1; do
    scons debug=$debug tests/unit && ./build/last/tests/unit/run
    scons debug=$debug run_e2e_tests
done
