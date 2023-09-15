#!/bin/bash

file="khovanov_03_crossings.csv"
touch "$file"
cat 03a_torus_khovanov.csv >> "$file"

for n in {04..17}; do
    oldfile="$file"
    file="khovanov_${n}_crossings.csv"
    touch "$file"
    cat "$oldfile" >> "$file"
    for x in `find . -iname "*$n*khovanov.csv"`; do
        cat "$x" >> "$file"
    done
done
