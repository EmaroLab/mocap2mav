#!/usr/bin/env bash
BDIR="build"

function b {
    cd $BDIR
    cmake .. && make
    cd ..
}

#TODO: add lcm auto generation
if [ ! -d $BDIR ]; then
  mkdir build
  b
else
  b
fi

#Export build dir as an absolute path
BABS=$(readlink -f build)
LABS=$(readlink -f lists)

function abspath {
        touch .direnv/build.txt
        touch .direnv/lists.txt
        echo $BABS > .direnv/build.txt
        echo $LABS > .direnv/lists.txt
}

#Create hidden files with links
if [ ! -d .direnv ]; then
        mkdir .direnv
        abspath
else
        rm .direnv/*
        abspath
fi

