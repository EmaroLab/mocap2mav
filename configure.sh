#!/usr/bin/env sh
sudo apt-get install libglib2.0-dev libeigen3-dev build-essentials
cd projectLIBS/lcm &&
./bootstrap.sh &&
./configure &&
make &&
sudo make install &&
sudo ldconfig

