#!/usr/bin/env bash
./gbfile-read 
./gbfile-read
./gbfile-mmap-random-write -s -a -h

for i in {1..19} 
do
./gbfile-read
./gbfile-read
./gbfile-mmap-random-write -s -a
done

for i in {1..20} 
do
./gbfile-read
./gbfile-read
./gbfile-mmap-random-write -p -a
done