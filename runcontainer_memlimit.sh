#! /usr/bin/env bash
docker run -it --name lab1mem --cpuset-cpus="0,1" --memory="500m" --memory-swap="1.5g" -v /home/asmita/OSLab1:/OSLab1 asmitaubuntu:22.04 /bin/bash