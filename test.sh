#!/bin/sh

INPUT=$1

# TODO: params with arguments

N_iters="1 10 100 1000 10000 100000 1000000"
normal_std="0.01 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 0.99"
T_init="1000"

mkdir -p tmp

for N in $N_iters
do
    for std in $normal_std
    do
        for T in $T_init
        do
            OUTPUT="tmp/output_${N}_${std}_${T}.png"
            ./cmkv $INPUT $OUTPUT $N $std $T
        done
    done
done

convert -delay 10 -loop 0 $(ls -rc tmp/*.png) markov.gif
