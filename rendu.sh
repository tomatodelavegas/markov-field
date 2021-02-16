#!/bin/bash

FILES="AUTHORS Makefile src/*.cc src/*.hh"

if ! stat "README.pdf"; then
    echo "NO README.pdf !"
else
    FILES="${FILES} README.pdf"
fi

zip "rendu-$(date +%s).zip" $FILES
