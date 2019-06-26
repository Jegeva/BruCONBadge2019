#! /bin/bash

docker build .
docker tag `docker images|egrep 'seconds? ago'|head -n 1|tr -s ' '|cut -d' ' -f3 ` esp32
