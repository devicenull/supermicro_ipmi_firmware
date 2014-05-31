#!/bin/sh
TARGETDIR=${1:-.}
HOSTNAME=`uname -n`
DO_SERVER=yes
DO_CLIENT=yes

trap "rm -f /var/tmp/key.pem /var/tmp/cert.pem /var/tmp/ssl.cnf" exit

echo "Generating SSL certificates in $TARGETDIR"

if [ -f $TARGETDIR/server.pem ]
then
    echo "WARNING: server.pem SSL Certificate file already exists."
    echo "         old file will be kept intact."
    DO_SERVER=no
fi

if [ -f $TARGETDIR/client.pem ]
then
    echo "WARNING: client.pem SSL Certificate trust store already exists."
    echo "         old file will be kept intact."
    DO_CLIENT=no
fi

if [ $DO_SERVER = no -a $DO_CLIENT = no ]
then
    exit 0
fi

cat > /var/tmp/ssl.cnf <<EOF
[req]
distinguished_name=user_dn
prompt=no

[user_dn]
CN=$HOSTNAME
emailAddress=root@$HOSTNAME
EOF

openssl req -x509 -days 365 -newkey rsa:2048 \
   -nodes -config /var/tmp/ssl.cnf   \
   -keyout /var/tmp/key.pem -out /var/tmp/cert.pem

chmod 700 /var/tmp/*.pem

if [ $DO_SERVER = yes ]
then
    cp /var/tmp/cert.pem $TARGETDIR/server.pem
    cp /var/tmp/key.pem $TARGETDIR/file.pem
    chmod 400 $TARGETDIR/server.pem $TARGETDIR/file.pem
fi

if [ $DO_CLIENT = yes ]
then
    cp /var/tmp/cert.pem $TARGETDIR/client.pem
    chmod 400 $TARGETDIR/client.pem
fi
