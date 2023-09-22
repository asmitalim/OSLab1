#!/usr/bin/env bash

./inputoutput -s -r -h
for i in {1..4} 
do
./inputoutput -s -r
done

for i in {1..5} 
do
./inputoutput -s 
done

for i in {1..5} 
do
./inputoutput -r
done

for i in {1..5} 
do
./inputoutput
done