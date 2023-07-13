#!/bin/bash

for x in *.csv; do
    tail -n +2 $x > tmp.csv;
    mv tmp.csv $x;
done
