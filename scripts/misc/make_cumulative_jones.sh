#!/bin/bash

file="jones_03_crossings.csv"
touch "$file"
cat 03a_torus_jones.csv >> "$file"

for n in {04..19}; do
    oldfile="$file"
    file="jones_${n}_crossings.csv"
    touch "$file"
    cat "$oldfile" >> "$file"
    for x in `find . -iname "*$n*jones.csv"`; do
        cat "$x" >> "$file"
    done
done
