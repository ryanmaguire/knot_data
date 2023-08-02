#!/bin/bash

file="dt_code_03_crossings.csv"
touch "$file"
cat ../raw/03a_torus_dt.csv >> "$file"

for n in {04..19}; do
    oldfile="$file"
    file="dt_code_${n}_crossings.csv"
    touch "$file"
    cat "$oldfile" >> "$file"
    for x in `find ../raw/ -iname "*$n*"`; do
        cat "$x" >> "$file"
    done
done
