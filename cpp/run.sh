#!/usr/bin/env bash

LD_LIBRARY_PATH=/usr/lib/jvm/java-17-openjdk-amd64/lib/server:../cppLib/build build/cpp "$@"
