#!/bin/bash
echo "file name: " $1
arm-linux-gnueabihf-gcc $1 -o ./output/test
