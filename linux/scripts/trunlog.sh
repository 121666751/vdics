###############################################
##
##定期备份日志文件
##Author: Fan Pengfei						
##
###############################################

#!/bin/bash

source ~/.bash_profile

GSIP_LOG_PATH=/tmp

echo "mv $GSIP_LOG_PATH/tmsrv.log "$GSIP_LOG_PATH/`date +"tmsrv.%Y-%m-%d"`.log""
mv $GSIP_LOG_PATH/tmsrv.log "$GSIP_LOG_PATH/`date +"tmsrv.%Y-%m-%d"`.log"

echo "mv $GSIP_LOG_PATH/pvsrv.log "$GSIP_LOG_PATH/`date +"pvsrv.%Y-%m-%d"`.log""
mv $GSIP_LOG_PATH/pvsrv.log "$GSIP_LOG_PATH/`date +"pvsrv.%Y-%m-%d"`.log"

echo "mv $GSIP_LOG_PATH/pmsrv.log "$GSIP_LOG_PATH/`date +"pmsrv.%Y-%m-%d"`.log""
mv $GSIP_LOG_PATH/pmsrv.log "$GSIP_LOG_PATH/`date +"pmsrv.%Y-%m-%d"`.log"

echo "mv $GSIP_LOG_PATH/svsrv.log "$GSIP_LOG_PATH/`date +"svsrv.%Y-%m-%d"`.log""
mv $GSIP_LOG_PATH/svsrv.log "$GSIP_LOG_PATH/`date +"svsrv.%Y-%m-%d"`.log"

echo "mv $GSIP_LOG_PATH/smsrv.log "$GSIP_LOG_PATH/`date +"smsrv.%Y-%m-%d"`.log""
mv $GSIP_LOG_PATH/smsrv.log "$GSIP_LOG_PATH/`date +"smsrv.%Y-%m-%d"`.log"
