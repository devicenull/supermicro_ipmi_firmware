#! /bin/bash

#sed -i -e '/\/\/.*include/d' $1
#sed -i -e 's/#include.*\//#include </' \
#	-e 's/.h\"/.h>/' $1

sed -i -e '/\/\/.*include/d' $1
sed -i -e 's/#include ".*\//#include  "/' $1

sed -i -e ":begin; /\*.*\$Log:/,/\*\*\*/ \
	{/\*\*\*/! { $! { N; b begin }; }; \
	s/\*.*\$Log: IncSticPath.sh,v $
	s/\*.*\Revision 1.5  2008/09/01 06:00:18  kenntrf
	s/\*.*\Modify By Kent
	s/\*.*\1

#sed -i -e ":begin; /\*.*Revision/,/\*\*\*/ \
#	{/\*\*\*/! { $! { N; b begin }; }; \
#	s/\*.*Revision.*\*\*\*/\*/; };" $1

#sed -i -e ":begin; /Copyright/,/\$Source:/ \
#	{ /\$Source: /home/cvsroot/SM_SRC/PKConfig/Lx_Script/IncSticPath.sh,v $! { N; b begin };}; \
#	/\$Source: /home/cvsroot/SM_SRC/PKConfig/Lx_Script/IncSticPath.sh,v $1

sed -i -e ":begin; /\*.*Copyright/,/\*\*\*/ \
	{ /\*\*\*/! { $! { N; b begin };}; \
	/\*\*\*/d; };" $1
	
sed -i -e ":begin; /\$RCSfile:/,/\*\*\*/ \
	{ /\*\*\*/! { $! { N; b begin };}; \
	/\*\*\*/d; };" $1