#!/bin/sh
CONFIG_GPL_BUILD=1
export CONFIG_GPL_BUILD
make
echo "" > .build_done
