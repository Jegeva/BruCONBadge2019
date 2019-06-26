#! /bin/bash

if [ "$#" -ne 1 ]; then
    DEV="/dev/ttyUSB0"
else
    DEV=$1
fi

LOC_PATH=$(readlink -f $0|sed -r 's/[^/]+\/[^/]+$//')
echo
echo "Mapped Path  : $LOC_PATH"
echo "Mapped Device: $DEV"
echo
docker run -t -i -v "$LOC_PATH:/root/BruCON2019:rw" --device $DEV esp32 /bin/bash