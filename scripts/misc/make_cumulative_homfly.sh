#!/bin/bash

file="homfly_03_crossings.csv"
touch "$file"
cat 03a_torus_homfly.csv >> "$file"

for n in {04..19}; do
    oldfile="$file"
    file="homfly_${n}_crossings.csv"
    touch "$file"
    cat "$oldfile" >> "$file"
    for x in `find . -iname "*$n*homfly.csv"`; do
        cat "$x" >> "$file"
    done
done
