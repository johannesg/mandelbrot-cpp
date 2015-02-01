#!/bin/sh
DEPDIR="$1"
DIR="$2"
shift 3
case "$DIR" in "" | ".")
	g++ -MM -MG "$@" | sed -e "s@^\(.*\)\.o:@\1.d \1.o:@"
;;
*)
	g++ -MM -MG "$@" | sed -e "s@^\(.*\)\.o:@$DEPDIR/\1.d $DEPDIR/\1.o:@"
;;
esac
