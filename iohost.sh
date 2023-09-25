#!/usr/bin/env bash

./inputoutput -s -r -h
for i in {1..9} 
do
./inputoutput -s -r
done

for i in {1..10} 
do
./inputoutput -s 
done

for i in {1..10} 
do
./inputoutput -r
done

for i in {1..10} 
do
./inputoutput
done