#!/bin/bash

echo "Enter package name"

read name
cd Modules

mkdir $name



mkdir $name/build
mkdir $name/src
mkdir $name/include
mkdir $name/dir
mkdir $name/libs

cd ..


