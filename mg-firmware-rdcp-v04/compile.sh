#!/bin/bash

arduino-cli compile -v --fqbn "esp32:esp32:esp32s3:CDCOnBoot=cdc,FlashSize=16M,PartitionScheme=app3M_fat9M_16MB" --output-dir build ./
