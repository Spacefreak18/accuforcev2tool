#!/bin/bash
#

test -c "$1" || { echo "Must specify a device" && exit 222; }


printf '\x02\x00\x58\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7d\x03' > "$1"
