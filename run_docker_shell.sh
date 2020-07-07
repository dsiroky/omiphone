#!/bin/bash
set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

docker build -t omiphone .
docker run \
  -it \
  --rm \
  --mount type=bind,source="$(pwd)",target=/app \
  -e SCONSFLAGS="-Q -u -j$(nproc)" \
  omiphone:latest \
  /bin/bash
