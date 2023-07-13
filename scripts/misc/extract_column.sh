#!/bin/bash

for x in *.csv; do
    mv $x tmp.csv;
    awk -F "\"*,\"*" '{print $2}' tmp.csv >> $x;
    rm -f tmp.csv;
done
