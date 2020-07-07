# Omiphone

Process transcribed number groups, generate and validate possible phone number
interpretations.

## Environment setup

1. Execute `git submodule update --init --recursive`.
1. Execute `./run_docker_shell.sh` to obtain a build environment. Following
commands are meant to be run in the docker shell.

## Build

Run `scons -h` to get build options.
Run `scons` to build all.

## Execution

Input must contain only numbers separated by single spaces. Otherwise it will
lead to undefined behavior. Input is not sanitized. Program is using standard
input to obtain data. The whole group must be in quotes. E.g.:

`echo "0 0 30 69 700 24 1 3 50 2" | ./build/last/src/omiphone/omiphone`

Standard input was chosen for potential future extension to pipeline more than
one group.

## Tests

Most core functions are covered by unittests. There are only few end-to-end tests
to make sure the program works as a whole.

Execute `./run_all_tests.sh`.

## Doxygen

1. Execute `./build_doxygen.sh`
1. Open `build/doxygen/html/index.html`.

## Architecture

The implementation is using return-value pipelining which makes it easy to
write simple unittests. Good enough for single runs. If it was supposed to be a
high performance system with minimal memory allocations and fragmentation,
callback pipelining would be better.
