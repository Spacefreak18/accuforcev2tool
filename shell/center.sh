#!/bin/bash
#

test -c "$1" || { echo "Must specify a device" && exit 222; }


printf '\x02\x01\x57\x01\x84\x03\x10\x00\xFF\x01\x00\x06\x06\x06\x7d\x03' > "$1"
