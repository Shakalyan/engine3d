#!/bin/bash
CPATH=./src
HPATH=./src

exists=`find $CPATH $HPATH -name "$1.[ch]"`
if test "$exists"; then
    echo "Module '$1' already exists"
    exit 1
fi

header=$HPATH/$1.h
cfile=$CPATH/$1.c
modguard=${1^^}_GUARD

echo "#ifndef $modguard" >> $header
echo "#define $modguard" >> $header
echo -n "#endif" >> $header

echo -n "#include \"$1.h\"" >> $cfile