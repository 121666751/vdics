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

cd ../sasrv
make clean

cd ../pasrv
make clean

cd ../gsipsrv
make clean

echo "clean over!"

cd ../dbolayer

echo "start build dboalyer"

make && make install

echo "start build tmsrv"
cd ../tmsrv
make && make install

echo "start build pvsrv"
cd ../pvsrv
make && make install

echo "start build pmsrv"
cd ../pmsrv
make && make install

echo "start build pasrv"
cd ../pasrv
make && make install

echo "start build svsrv"
cd ../svsrv
make && make install

echo "start build smsrv"
cd ../smsrv
make && make install

echo "start build sasrv"
cd ../sasrv
make && make install


echo "start build gsipsrv"
cd ../gsipsrv
make && make install

echo "start to copy all scripts"

cd ../scripts
cp * -R ../bin

echo "start to copy so to lib"
cd ../usemodel/lib/
cp * ../../lib/

echo "start to copy lib to bin"
cd ../../lib
cp * ../bin 
echo "start to copy table.sql to bin/backup"
cd ..
cp ../../../../database/Framework/table.sql bin/backup/table.sql 

