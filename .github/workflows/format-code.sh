#!/bin/sh

CLANGFORMAT=`which clang-format`
STYLE="file"
FILES="source/*.cpp"

if [ $# -eq 1 ]; then
    if [[ $1 == "-h" || $1 == "--help" ]]; then
        echo "USAGE"
        echo -e "    ./format-code.sh [-h] [-f]\n"
        echo "DESCRIPTION"
        echo -e "    -h, --help\tShows this message."
        echo -e "    -f, --fix\tMake formatting changes."
        exit 0
    fi
    if [[ $1 == "-f" || $1 == "--fix" ]]; then
        $CLANGFORMAT -i --style=${STYLE} ${FILES}
        exit 0
    fi
    exit 1
fi

$CLANGFORMAT -i -n --Werror --style=${STYLE} ${FILES}