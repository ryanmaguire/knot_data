#!/bin/bash

# Replaces hyphens in a filename with underscores.
for x in $1/*.csv; do
    y=${x//[-]/_};
    mv $x $y;
done
