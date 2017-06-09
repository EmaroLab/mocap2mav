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
PABS=$(readlink -f .)

function abspath {
        touch .direnv/build.txt
        touch .direnv/lists.txt
        touch .direnv/proje.txt
        echo $BABS > .direnv/build.txt
        echo $LABS > .direnv/lists.txt
        echo $PABS > .direnv/proje.txt
        #bin helper, ensures that binaries can be executed fromm bin dir
        cp -r .direnv $BABS/bin/.direnv
}

#Create hidden files with links
if [ ! -d .direnv ]; then
        mkdir .direnv
        abspath
else
        rm .direnv/*
        abspath
fi

