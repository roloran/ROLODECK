#!/bin/bash

set -euo pipefail

if [ $# -ne 1 ]; then
    echo "Usage: $0 <serial-device>"
    exit 1
fi

DEVICE="$1"

if ! arduino-cli board list | awk '{print $1}' | grep -Fxq "$DEVICE"; then
    echo "Error: Device '$DEVICE' not found."
    echo
    echo "Available devices:"
    arduino-cli board list
    exit 1
fi

arduino-cli upload \
    --fqbn "esp32:esp32:esp32s3:CDCOnBoot=cdc,FlashSize=16M,PartitionScheme=app3M_fat9M_16MB" \
    --port "$DEVICE" \
    --input-dir build