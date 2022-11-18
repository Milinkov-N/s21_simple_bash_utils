#!bin/bash

docker build -t "valgrind:1.0" . && docker run -it -v $PWD:/tmp -w /tmp valgrind:1.0