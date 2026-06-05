#!/usr/bin/env sh

cmake -B build -DCMAKE_INSTALL_PREFIX=extern -DCMAKE_SYSTEM_NAME=Windows
cmake --build build --target install --config Debug