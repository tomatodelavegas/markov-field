#!/bin/sh

if [ "$#" -lt 1 ]; then
    echo "Illegal number of parameters"
    exit 2
fi

INPUT=$1

# TODO: params with arguments

N_iters="1000 10000 100000 1000000 5000000"
T_init="1000 10000 100000"
T_dec_factor="0.99 0.999 0.9999 0.999999"
normal_std="0.01 0.1 1"

mkdir -p tmp
rm tmp/*

make -j

for N in $N_iters
do
    echo "N = $N"
    for T in $T_init
    do
        echo "  T = $T"
        for T_dec in $T_dec_factor
        do
            for std in $normal_std
            do
                OUTPUT="tmp/output_${N}_${T}_${T_dec}_${std}.png"
                ./cmkv $INPUT $OUTPUT $N $T $T_dec $std
            done
        done
    done
done

convert -delay 10 -loop 0 $(ls -rc tmp/*.png) markov.gif
