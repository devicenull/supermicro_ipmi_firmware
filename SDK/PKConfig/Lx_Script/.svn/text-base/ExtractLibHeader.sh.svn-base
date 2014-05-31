#! /bin/bash

CurrentFile=$(echo $2 | sed 's/.*\///')

ExtractObjC=$(echo $1|sed 's/\..*/\.c/')
CurrentFileC=$(echo $CurrentFile|sed 's/\..*/\.c/')

if [ "$ExtractObjC" == "$CurrentFileC" ]; then

	mkdir $3/TmpLibExSDK
	mkdir $3/TmpLibExSDK/inc
SourcePath=$(grep -l AST2050 "./"$(find ../ -name $ExtractObjC))
if [ -z "$SourcePath" ]; then
  SourcePath=$(find ../ -name $ExtractObjC)
fi;
  cp $SourcePath "$3/TmpLibExSDK/"

	echo "$3/TmpLibExSDK/$ExtractObjC"
	

	/bin/bash $3/PKConfig/Lx_Script/ExtractHeader.sh $SourcePath "$3/TmpLibExSDK/inc"
	
	/bin/bash $3/PKConfig/Lx_Script/IncSticPath.sh "$3/TmpLibExSDK/$ExtractObjC"
	
  for FILE in $3/TmpLibExSDK/inc/*
  do
      /bin/bash $3/PKConfig/Lx_Script/IncSticPath.sh $FILE
  done
	echo $SourcePath
fi;