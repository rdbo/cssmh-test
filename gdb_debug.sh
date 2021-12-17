#!/bin/bash

GAME_NAME=hl2_linux
LIB_PATH=$(pwd)/build/libcssmh.so
LOAD_MODE=1 # RTLD_LAZY

if [ $EUID -ne 0 ]; then
	echo "[!] Please run as root"
	exit -1
fi

if [ ! -f $LIB_PATH ]; then
	echo "[!] Library does not exist"
	exit -1
fi

gdb -n -q \
	-ex "attach $(pidof -s $GAME_NAME)" \
	-ex "set \$dlopen = (void *(*)(char *, int))dlopen" \
	-ex "call \$dlopen(\"$LIB_PATH\", $LOAD_MODE)" \
