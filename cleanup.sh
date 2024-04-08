#!/usr/bin/env bash

for i in cppLib cpp java; do
  pushd $i; ./cleanup.sh; popd
done
