#!/usr/bin/env sh
sudo apt-get install libglib2.0-dev &&
sudo apt-get install libeigen3-dev &&
sudo apt-get install build-essential &&
cd projectLIBS/lcm &&
./bootstrap.sh &&
./configure &&
make &&
sudo make install &&
sudo ldconfig

