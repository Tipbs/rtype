#!/bin/sh

CLANGFORMAT=`which clang-format`
STYLE="file"
SERVER_FILES="server/source/*.cpp"
GUI_FILES="gui/source/*.cpp"

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
        $CLANGFORMAT -i --style=${STYLE} ${GUI_FILES} ${SERVER_FILES}
        exit 0
    fi
    exit 1
fi

echo "----- SERVER/ -----"
$CLANGFORMAT -n --Werror --style=${STYLE} ${SERVER_FILES}
if [ $? -ne 0 ]; then
    exit 1
fi
echo "Nothing to format"

echo "----- GUI -----"
$CLANGFORMAT -n --Werror --style=${STYLE} ${GUI_FILES}
if [ $? -ne 0 ]; then
    exit 1
fi
echo "Nothing to format"
