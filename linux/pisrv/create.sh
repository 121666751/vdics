#!/bin/bash

cd ../dbcommon
make clean 
make 
make install
cd -
make clean
make
make install
