#!/usr/bin/env bash
sudo guestmount -a /home/asmita/OSLab0/bootimages/debnocloud.qcow2 -i /home/asmita/vmdiskmount
echo "Mounted folder"
make clean
sudo mkdir -p /home/asmita/vmdiskmount/OSLab1
sudo cp /home/asmita/OSLab1/*.c /home/asmita/vmdiskmount/OSLab1/
sudo cp /home/asmita/OSLab1/*.sh /home/asmita/vmdiskmount/OSLab1/
sudo cp /home/asmita/OSLab1/makefile /home/asmita/vmdiskmount/OSLab1/
sudo cp /home/asmita/OSLab1/*.h /home/asmita/vmdiskmount/OSLab1/
echo "Copied files"
sudo fusermount -u /home/asmita/vmdiskmount
echo "Unmounted"
