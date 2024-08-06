#!/bin/bash
echo "file name: " $1
bear -- gcc $1 -o ./output/test
# arm-linux-gnueabihf-gcc $1 -o ./output/test
