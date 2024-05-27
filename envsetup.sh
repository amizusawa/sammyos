#!/bin/bash

CURRENT_DIR=$(pwd)
BASENAME=$(echo ${CURRENT_DIR} | grep -oE '[^/]+$')
if [ ${BASENAME} != "sammyos" ]; then
    echo "Please run \". envsetup.sh\" from the sammyos root directory. Aborting"
    return 1
else
    export SAMMYOSROOT=${CURRENT_DIR}
    echo "SAMMYOSROOT set to ${SAMMYOSROOT}"
fi

if [ -z "$1" ]; then
    export SAMMYOSARCH=i386
    echo "Target architecture not specified. Defaulting to i386"
elif [ $1 == "i386" ]; then
    export SAMMYOSARCH=i386
    echo "Setting target architecture to i386."
else
    echo "Unknown target architecture. Aborting."
    return 1
fi

echo "You are able to use \"smk\" to build sammyos from any directory."
smk() {
    cd ${SAMMYOSROOT}
    make $1
    cd -
}
