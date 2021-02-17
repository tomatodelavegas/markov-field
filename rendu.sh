#!/bin/bash

FILES="AUTHORS Makefile src/*.cc src/*.hh"

if ! stat "README.pdf" > /dev/null; then
    echo -e "\e[91mNO README.pdf !\e[0m"
else
    FILES="${FILES} README.pdf"
fi

FILENAME="rendu-$(date +%s).zip"

zip $FILENAME $FILES && echo -e "\e[92mzip CREATION: OK\e[0m"

function check_rendu() {
    cd $(mktemp -d)
    unzip "$OLDPWD/$1" -d .
    if ! make; then
        echo -e "\e[91mFatal Error, could not make !\e[0m"
        exit 1
    fi
    if [ "$#" -ge 2 ]; then
        ./cmkv $2 out.png
        if ! stat "out.png" > /dev/null; then
            echo -e "\e[91mNo output !\e[0m"
            exit 1
        else
            echo -e "output file: \e[95m$PWD/out.png\e[0m"
            echo -e "\e[92mzip check: OK\e[0m"
        fi
    else
        echo -e "\e[91mNo input provided, cannot test output file !\e[0m"
    fi
    cd -
}

# first arg must be absolute
check_rendu $FILENAME $1
