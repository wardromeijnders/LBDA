#!/usr/bin/env bash

# Fixes all non-conforming source and header files in the smips/ directory.
find smips/ -iname "*.h" -o -iname "*.cpp" | xargs clang-format -i

# Get all files modified by clang-format.
dirty=$(git ls-files --modified)

if [[ $dirty ]];
then
  echo "Files with unexpected source code formatting:"
  echo $dirty
  exit 1
fi
