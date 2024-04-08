#!/usr/bin/env bash

set -e

for i in java cppLib cpp; do
  pushd $i; ./build.sh; popd
done
