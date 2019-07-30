#! /bin/bash
# swaps planted with backup
PLANTED=$1"_planted"

echo "$PLANTED.h <=> $PLANTED.h.backup"
echo "$PLANTED.c <=> $PLANTED.c.backup"
read -rp $'Continue? (Y/n): ' key;

echo $key

if [ "$key" == "Y" ]; then
	mv $PLANTED.h.backup $PLANTED.h.swap
	mv $PLANTED.h        $PLANTED.h.backup
	mv $PLANTED.h.swap   $PLANTED.h
	touch $PLANTED.h

	mv $PLANTED.c.backup $PLANTED.c.swap
	mv $PLANTED.c        $PLANTED.c.backup
	mv $PLANTED.c.swap   $PLANTED.c
	touch $PLANTED.c

	echo "Done"
else
	echo "Aborted"
fi

read -p "Press <ENTER> to exit."

