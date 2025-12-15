#!/bin/sh

LEDGER_TARGETS="nanox nanos2 stax flex apex_p apex_m"

for i in $LEDGER_TARGETS; do
    echo "Building for TARGET=$i"
    make TARGET="$i"
done
