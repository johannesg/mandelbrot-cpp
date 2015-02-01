#!/bin/sh
DEPDIR="$1"
DIR="$2"
shift 3
case "$DIR" in "" | ".")
	gcc -MM -MG "$@" | sed -e "s@^\(.*\)\.o:@\1.d \1.o:@"
;;
*)
#	echo "DEPDIR = $DEPDIR" >&2
#	echo "DIR = $DIR" >&2
#	echo "CMD = $@" >&2
	mkdir -p $DEPDIR
	gcc -MM -MG "$@" | sed -e "s@^\(.*\)\.o:@$DEPDIR/\1.d $DEPDIR/\1.o:@"
;;
esac
