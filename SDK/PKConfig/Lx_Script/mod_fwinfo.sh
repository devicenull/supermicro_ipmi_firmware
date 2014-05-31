#!/bin/bash
ConFile_Path=`pwd`/FileSystem/Host/HERMON/Board/SuperMicro_X7SB3
ConFiles=`ls  $ConFile_Path/platform_*.xml`
date=`date '+%Y-%m-%d%n'`

echo "please input tagvalue[BXXX]"
read tagvalue

for i in $ConFiles
do
    mv $i $i.old
    echo "Modify file ${i##/*/}  ...."
    cat  $i.old |  sed -e "s/[0-9]\{4\}-[0-1][0-9]-[0-3][0-9]_B[0-9]*/${date}_${tagvalue}/" >>$i
    rm $i.old 
done

