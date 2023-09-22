FROM ubuntu:22.04
WORKDIR /asmita
COPY *.c .
COPY *.h .
COPY makefile .
COPY *.sh .
RUN apt update
RUN apt install -y gcc
RUN apt install -y make
RUN apt install -y tree
RUN make clean
RUN make


