###############################################
##
##定期备份日志文件
##Author: Fan Pengfei						
##
###############################################

#!/bin/bash

source ~/.bash_profile

GSIP_LOG_PATH=/tmp
LOG_PREFIX=$2
DIR_NAME=$1

if [ -z $1 ]
then
	echo "Usage baklog.sh dirname log_prefix_name"
	exit
fi

if [ -z $2 ]
then
	echo "Usage baklog.sh dirname log_prefix_name"
	exit
fi

echo "$GSIP_LOG_PATH/$DIR_NAME"
mkdir $GSIP_LOG_PATH/$DIR_NAME

echo "cp $GSIP_LOG_PATH/tmsrv.log" $GSIP_LOG_PATH/$DIR_NAME/$LOG_PREFIX"_tmsrv.log"
cp $GSIP_LOG_PATH/tmsrv.log $GSIP_LOG_PATH/$DIR_NAME/$LOG_PREFIX"_tmsrv.log"

echo "cp $GSIP_LOG_PATH/pvsrv.log" $GSIP_LOG_PATH/$DIR_NAME/$LOG_PREFIX"_pvsrv.log"
cp $GSIP_LOG_PATH/pvsrv.log $GSIP_LOG_PATH/$DIR_NAME/$LOG_PREFIX"_pvsrv.log"

echo "cp $GSIP_LOG_PATH/pmsrv.log" $GSIP_LOG_PATH/$DIR_NAME/$LOG_PREFIX"_pmsrv.log"
cp $GSIP_LOG_PATH/pmsrv.log $GSIP_LOG_PATH/$DIR_NAME/$LOG_PREFIX"_pmsrv.log"

echo "cp $GSIP_LOG_PATH/svsrv.log" $GSIP_LOG_PATH/$DIR_NAME/$LOG_PREFIX"_svsrv.log"
cp $GSIP_LOG_PATH/svsrv.log $GSIP_LOG_PATH/$DIR_NAME/$LOG_PREFIX"_svsrv.log"

echo "cp $GSIP_LOG_PATH/smsrv.log "$GSIP_LOG_PATH/$DIR_NAME/$LOG_PREFIX"_smsrv.log"
cp $GSIP_LOG_PATH/smsrv.log $GSIP_LOG_PATH/$DIR_NAME/$LOG_PREFIX"_smsrv.log"

echo "cp $GSIP_LOG_PATH/sasrv.log "$GSIP_LOG_PATH/$DIR_NAME/$LOG_PREFIX"_sasrv.log"
cp $GSIP_LOG_PATH/sasrv.log $GSIP_LOG_PATH/$DIR_NAME/$LOG_PREFIX"_sasrv.log"

echo "cp $GSIP_LOG_PATH/pasrv.log "$GSIP_LOG_PATH/$DIR_NAME/$LOG_PREFIX"_pasrv.log"
cp $GSIP_LOG_PATH/pasrv.log $GSIP_LOG_PATH/$DIR_NAME/$LOG_PREFIX"_pasrv.log"

echo "bakup logs ok."
