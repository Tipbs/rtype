#!/bin/sh

CLANGFORMAT=`which clang-format`
STYLE="file"
FILES="source/*.cpp include/*.hpp"

$CLANGFORMAT -i --style=${STYLE} ${FILES}
