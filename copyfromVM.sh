#!/usr/bin/env bash
sudo guestmount -a /home/asmita/OSLab0/bootimages/debnocloud.qcow2 -i /home/asmita/vmdiskmount
echo "Mounted folder"
make clean
sudo cp /home/asmita/vmdiskmount/OSLab1/*.txt /home/asmita/results
echo "Copied files"
sudo fusermount -u /home/asmita/vmdiskmount
echo "Unmounted"
