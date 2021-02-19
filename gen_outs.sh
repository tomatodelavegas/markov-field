#!/bin/sh

if [ "$#" -lt 1 ]; then
    echo "Illegal number of parameters"
    exit 2
fi

INPUT=$1

N=10000000
T=1000000
T_dec=0.9999

mul_bin=1
muls_diag="0 1"
muls_hor="0 1"
muls_neigh="0 1"
muls_corners="0 1"

mkdir -p tmp
rm tmp/*

make

for mul_diag in $muls_diag
do
    echo "mul_diag = $mul_diag"
    for mul_hor in $muls_hor
    do
        echo " mul_hor = $mul_hor"
        for mul_neigh in $muls_neigh
        do
            echo "  mul_neigh = $mul_neigh"
            for mul_corners in $muls_corners
            do
                echo "   mul_corners = $mul_corners"
                OUTPUT="tmp/b${mul_bin}_d${mul_diag}_h${mul_hor}_n${mul_neigh}_c${mul_corners}.png"
                ./cmkv $INPUT $OUTPUT $N $T $T_dec $mul_bin $mul_diag $mul_hor $mul_neigh $mul_corners
            done
        done
    done
done

convert -delay 10 -loop 0 $(ls -rc tmp/*.png) markov.gif
