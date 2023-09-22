#! /usr/bin/env bash
docker run -it --name lab1asmi --cpuset-cpus="0,1" --memory="2g" -v /home/asmita/OSLab1:/OSLab1 asmitaubuntu:22.04 /bin/bash