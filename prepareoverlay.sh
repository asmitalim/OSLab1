#! /usr/bin/env bash
sudo umount forcontainer/merged
echo "Unmounted"
rm -rf forcontainer 
mkdir forcontainer 
mkdir forcontainer/lower 
mkdir forcontainer/upper 
mkdir forcontainer/work 
mkdir forcontainer/merged
echo "Created directories"
truncate -s 1g forcontainer/lower/gb1.bin
echo "Created 1GB file"
sudo mount -t overlay overlay -o lowerdir=forcontainer/lower,upperdir=forcontainer/upper,workdir=forcontainer/work forcontainer/merged
echo "Mounted directories"
fincore -b forcontainer/lower/gb1.bin
make
./gbfile-read forcontainer/lower/gb1.bin
fincore -b forcontainer/lower/gb1.bin
