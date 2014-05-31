#!/bin/sh

if [ ! -f .build_done ];then
./BUILD.sh
fi
make install
