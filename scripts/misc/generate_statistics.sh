#!/bin/bash

echo "Name | Unique | Almost Unique | Total | Percent Unique"
for x in *count.txt; do
    unique=`grep '^      1' $x | wc -l`
    almost=`grep '^      2' $x | wc -l`
    total=`cat $x | wc -l`
    percent=`bc -l <<< "$unique/$total"`
    echo "$x | $unique | $almost | $total | $percent"
done
