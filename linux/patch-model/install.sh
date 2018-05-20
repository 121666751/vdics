#!/bin/bash

USER_NAME=gsip

PATCH_VERSION=1.0.0.5-3

PATCH_CONTENT="tmsrv"

echo "stop crontab"

crontab -l > vdics.cfg

crontab -r

echo "stop service"

pkill smsrv -U $USER_NAME

pkill tmsrv -U $USER_NAME 

pkill pvsrv -U $USER_NAME 

pkill pmsrv -U $USER_NAME

pkill svsrv -U $USER_NAME

pkill pasrv -U $USER_NAME

pkill sasrv -U $USER_NAME

echo "backup files ..."
echo "backup files is saved in /home/gsip/."
echo "backup file name is patch-"$PATCH_VERSION"_bak.tar.gz"

tar -czvf "patch-"$PATCH_VERSION"_bak".tar.gz  $PATCH_CONTENT
mv  "patch-"$PATCH_VERSION"_bak".tar.gz  /home/gsip/.

echo "installing patch ..."

cp $PATCH_CONTENT /home/gsip/vdics/.

echo "start crontab"

crontab < vdics.cfg

echo "start service"

/home/gsip/vdics/gsipsrv

echo "install patch-$PATCH_VERSION complete."
