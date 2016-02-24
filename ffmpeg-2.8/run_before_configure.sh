#!/bin/bash
#PREFIX=/opt
PREFIX=/usr/local
export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:$PREFIX/lib/pkgconfig"
export PATH="$PATH:$PREFIX/bin:$PREFIX/lib"

