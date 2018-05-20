#!/bin/bash

BACKDIR=~/vdics/backup/
echo $BACKDIR
cd $BACKDIR

NUM=1
BACKFILE="dat"`date +%y%m%d`"-$NUM"
while [ -f $BACKFILE".tar.gz" ]
do
	NUM=`expr $NUM + 1`
    BACKFILE="dat"`date +%y%m%d`"-$NUM"
done

mysqldump --add-drop-table -ugsip -pgsip0728  gsipdb > $BACKFILE

if [ ! $? = 0 ]; then
	echo "Dump database Failed!"
	exit 0
fi

tar cvfz $BACKFILE.tar.gz $BACKFILE
if [ ! $? = 0 ]; then
	echo "Compress Failed!"
	exit 0
fi
rm -f $BACKFILE

mysql -ugsip -pgsip0728 gsipdb << SQLIT
truncate bocctrl;
truncate pbcdata;
truncate vouchers;
truncate multis;
SQLIT

