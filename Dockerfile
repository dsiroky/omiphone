FROM ubuntu:20.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
        doxygen \
        g++ \
        graphviz \
        libboost-dev \
        python3-pytest \
        scons

WORKDIR /app
