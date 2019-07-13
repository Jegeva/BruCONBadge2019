#! /bin/bash

if [ "$#" -ne 1 ]; then
    DEV="/dev/ttyUSB0"
else
    DEV=$1
fi

READLINK=readlink
SED=sed

[ $(uname) = "Darwin" ] && {
    READLINK=greadlink
    SED=gsed
}

LOC_PATH=$($READLINK -f $0 | $SED -r 's/[^/]+\/[^/]+$//')
echo
echo "Mapped Path  : $LOC_PATH"
echo "Mapped Device: $DEV"
echo
docker run -t -i -v "$LOC_PATH:/root/BruCON2019:rw" --device $DEV esp32 /bin/bash
