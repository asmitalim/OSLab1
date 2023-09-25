#!/usr/bin/env bash
qemu-system-x86_64 -m 500 -vga virtio -cpu host -smp cores=2,threads=2 --enable-kvm -drive file=/home/asmita/OSLab0/bootimages/debnocloud.qcow2,format=qcow2