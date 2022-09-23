#!/bin/sh 
set -e
set -x 

pandoc -F pandoc-plot --lua-filter diagram-generator.lua --template eisvogel $1.md -o output_$1.pdf
