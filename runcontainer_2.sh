#! /usr/bin/env bash
docker run -it --name lab1asmi2 --cpuset-cpus="0,1" --memory="2g" -v /home/asmita/OSLab1/forcontainer/merged:/asmita/gbdir asmitaubuntu:22.04 /bin/bash