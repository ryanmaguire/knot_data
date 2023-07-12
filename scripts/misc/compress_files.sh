#!/bin/bash

# Compresses everything with default (level 6) compression.
for x in $1/*.csv; do
    xz -z $x;
done
