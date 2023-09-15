#!/bin/bash

echo "Cr     = Crossing number, largest number of crossings considered."
echo "Unique = Number of polynomials that occur for one knot."
echo "Almost = Number of polynomials that occur for two knots."
echo "Total  = Total number of distinct polynomials in list."
echo "Knots  = Total number of knots in list."
echo "FracU  = Unique / Total"
echo "FracT  = Total / Knots"
echo "FracK  = Unique / Knots"
echo ""
echo "Cr |  Unique   |  Almost  |   Total   |   Knot    |  FracU   |  FracT   |  FracK"
echo "---------------------------------------------------------------------------------"
for n in {03..19}; do
    x=$(find . -iname "*$n*.txt")
    y=$(find . -iname "*$n*.csv")
    unique=`grep '^      1' $x | wc -l`
    almost=`grep '^      2' $x | wc -l`
    total=`cat $x | wc -l`
    knot=`cat $y | wc -l`
    fracu=`bc -l <<< "$unique/$total" | awk '{printf "%f", $0}'`
    fract=`bc -l <<< "$total/$knot" | awk '{printf "%f", $0}'`
    frack=`bc -l <<< "$unique/$knot" | awk '{printf "%f", $0}'`
    printf "$n | %9d | %8d | %9d | %9d | $fracu | $fract | $frack\n" $unique $almost $total $knot
    # echo "$n | $unique | $almost | $total | $knot | $fracu | $fract | $frack"
done
