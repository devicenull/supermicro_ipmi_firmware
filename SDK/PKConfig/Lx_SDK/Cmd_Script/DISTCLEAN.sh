#!/bin/sh

make clean_all
rm -f .build_done
cp -f .config.default .config
cp -f autoconfig.h.default autoconfig.h
