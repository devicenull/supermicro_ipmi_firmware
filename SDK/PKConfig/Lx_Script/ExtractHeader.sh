#! /bin/bash

gcc -DMODULE $CFLAGS -MM $1 > tmpf0
sed 's/.*\.c/HF = /' tmpf0 > tmpf1
sed -i -e '/.*\.o/d' tmpf1
echo "tmpfx : $1" >> tmpf1
echo -e "\tcp \$(sort \$(HF)) $2" >> tmpf1
make -f tmpf1
rm -f tmpf*


