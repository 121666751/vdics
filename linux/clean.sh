#!/bin/bash

echo "start clean..."

cd dbolayer

make clean

cd ../tmsrv
make clean

cd ../pvsrv
make clean 

cd ../pmsrv
make clean

cd ../svsrv
make clean

cd ../smsrv
make clean

cd ../pasrv
make clean

cd ../sasrv
make clean

cd ../gsipsrv
make clean

echo "clean over!"
