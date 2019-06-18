#!/bin/sh
export DISPLAY=:0
cd /home/snuc/bentley/OpenFrameworks/BentleyApp/bin/
ret=1
while [ $ret -ne 0 ]; do
    ./BentleyApp
    ret=$?
done