#!/bin/bash

# this is a simple benchmarking script for Hantenna 

for i in {1..12}
do
   echo "Welcome $i times"
   export OMP_NUM_THREADS=$i
   ./Hantenna inp/trans.inp inp/cond.inp inp/points.inp inp/config.inp
done

sed -i -- 's/FHTKEY51/FHTKEY101/g' inp/config.inp 
for i in {1..12}
do
   echo "Welcome $i times"
   export OMP_NUM_THREADS=$i
   ./Hantenna inp/trans.inp inp/cond.inp inp/points.inp inp/config.inp
done

sed -i -- 's/FHTKEY101/FHTKEY201/g' inp/config.inp 
for i in {1..12}
do
   echo "Welcome $i times"
   export OMP_NUM_THREADS=$i
   ./Hantenna inp/trans.inp inp/cond.inp inp/points.inp inp/config.inp
done

sed -i -- 's/FHTKEY201/ANDERSON801/g' inp/config.inp 
for i in {1..12}
do
   echo "Welcome $i times"
   export OMP_NUM_THREADS=$i
   ./Hantenna inp/trans.inp inp/cond.inp inp/points.inp inp/config.inp
done

sed -i -- 's/ANDERSON801/QWEKEY/g' inp/config.inp 
for i in {1..12}
do
   echo "Welcome $i times"
   export OMP_NUM_THREADS=$i
   ./Hantenna inp/trans.inp inp/cond.inp inp/points.inp inp/config.inp
done

sed -i -- 's/QWEKEY/CHAVE/g' inp/config.inp 
for i in {1..12}
do
   echo "Welcome $i times"
   export OMP_NUM_THREADS=$i
   ./Hantenna inp/trans.inp inp/cond.inp inp/points.inp inp/config.inp
done
