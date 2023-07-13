#!/bin/bash

for x in *.csv; do
    y=${x%.*};
    sort $x | uniq -c >> "${y}_count.txt";
done
