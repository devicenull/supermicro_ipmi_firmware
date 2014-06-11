#!/bin/sh

usage() {
 echo "usage: $0 -newcert|-newreq|-newreq-nodes|-newca|-sign|-verify" >&2
}

if [ -z "$OPENSSL" ]; then OPENSSL=openssl; fi

if [ -z "$DAYS" ] ; then DAYS="-days 365" ; fi	# 1 year
CONFIG="/etc/ssl/openssl.cnf"
CADAYS="-days 1095"	# 3 years
REQ="$OPENSSL req $SSLEAY_CONFIG" 
CA="$OPENSSL ca $SSLEAY_CONFIG"
VERIFY="$OPENSSL verify"
X509="$OPENSSL x509"
PKCS12="openssl pkcs12"

if [ -z "$CATOP" ] ; then CATOP=/tmp/CA ; fi
CAKEY=./cakey.pem
CAREQ=./careq.pem
CACERT=./cacert.pem
START_DATE=
END_DATE=

RET=0

while [ "$1" != "" ] ; do
case $1 in
-\?|-h|-help)
    usage
    exit 0
    ;;
-newcert)
    # create a certificate
    $REQ -new -x509 -nodes -keyout newkey.pem -out newcert.pem $DAYS -config $CONFIG -batch
    RET=$?
    echo "Certificate is in newcert.pem, private key is in newkey.pem"
    ;;
-newreq)
    # create a certificate request
    $REQ -new -keyout newkey.pem -out newreq.pem $DAYS -config $CONFIG -batch
    RET=$?
    echo "Request is in newreq.pem, private key is in newkey.pem"
    ;;
-newreq-nodes) 
    # create a certificate request
    $REQ -new -nodes -keyout newreq.pem -out newreq.csr $DAYS -config $CONFIG -batch
    RET=$?
    echo "Request (and private key) is in newreq.pem"
    ;;
-newca)
    # if explicitly asked for or it doesn't exist then setup the directory
    # structure that Eric likes to manage things
    NEW="1"
	rm -rf ${CATOP};
    if [ "$NEW" -o ! -f ${CATOP}/serial ]; then
	# create the directory hierarchy
	mkdir -p ${CATOP}
	mkdir -p ${CATOP}/certs
	mkdir -p ${CATOP}/crl
	mkdir -p ${CATOP}/newcerts
	mkdir -p ${CATOP}/private
	touch ${CATOP}/index.txt
	echo "01" > ${CATOP}/serial
    fi
    if [ ! -f ${CATOP}/private/$CAKEY ]; then

	    echo "Making SSL certificate ..."
	    $REQ -new -nodes -keyout ${CATOP}/private/$CAKEY -config $CONFIG \
			   	-out ${CATOP}/$CAREQ  -batch  >/dev/null 2>&1
	    $CA -create_serial -out ${CATOP}/$CACERT -config $CONFIG -batch \
				-startdate ${START_DATE} -enddate ${END_DATE} \
			    -keyfile ${CATOP}/private/$CAKEY  -selfsign \
			    -extensions v3_ca -infiles ${CATOP}/$CAREQ >/dev/null 2>&1
		sed -n -i '/-----BEGIN/,/-----END/p' ${CATOP}/cacert.pem
		cat ${CATOP}/cacert.pem ${CATOP}/private/$CAKEY > /nv/server.pem
		rm -rf ${CATOP};
	    RET=$?
    fi
    ;;
-xsign)
    $CA -policy policy_anything -infiles newreq.pem
    RET=$?
    ;;
-pkcs12)
    if [ -z "$2" ] ; then
	CNAME="My Certificate"
    else
	CNAME="$2"
    fi
    $PKCS12 -in newcert.pem -inkey newreq.pem -certfile ${CATOP}/$CACERT \
	    -out newcert.p12 -export -name "$CNAME"
    RET=$?
    exit $RET
    ;;
-sign|-signreq)
    $CA -policy policy_anything -out newcert.pem -infiles newreq.pem
    RET=$?
    cat newcert.pem
    echo "Signed certificate is in newcert.pem"
    ;;
-signCA)
    $CA -policy policy_anything -out newcert.pem -extensions v3_ca -infiles newreq.pem
    RET=$?
    echo "Signed CA certificate is in newcert.pem"
    ;;
-signcert)
    echo "Cert passphrase will be requested twice - bug?"
    $X509 -x509toreq -in newreq.pem -signkey newreq.pem -out tmp.pem
    $CA -policy policy_anything -out newcert.pem -infiles tmp.pem
    RET=$?
    cat newcert.pem
    echo "Signed certificate is in newcert.pem"
    ;;
-verify)
    shift
    if [ -z "$1" ]; then
	    $VERIFY -CAfile $CATOP/$CACERT newcert.pem
	    RET=$?
    else
	for j
	do
	    $VERIFY -CAfile $CATOP/$CACERT $j
	    if [ $? != 0 ]; then
		    RET=$?
	    fi
	done
    fi
    exit $RET
    ;;
*)
    echo "Unknown arg $i" >&2
    usage
    exit 1
    ;;
esac
shift
done
exit $RET
