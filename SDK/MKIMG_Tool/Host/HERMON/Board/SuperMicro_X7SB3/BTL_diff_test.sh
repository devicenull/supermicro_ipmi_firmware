#!/bin/bash

sBTL=$1
tBTL=$2
diff_test=`diff $sBTL $tBTL`

echo "src = $sBTL"
echo "target = $tBTL"

if [ -z "$diff_test" ]; then
	echo "Same BTL version.";
else
	echo "Different BTL version."
	cp -af $sBTL $tBTL
fi
