#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

TAONAD=${TAONAD:-$SRCDIR/taonad}
TAONACLI=${TAONACLI:-$SRCDIR/taona-cli}
TAONATX=${TAONATX:-$SRCDIR/taona-tx}
TAONAQT=${TAONAQT:-$SRCDIR/qt/taona-qt}

[ ! -x $TAONAD ] && echo "$TAONAD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
TNAVER=($($TAONACLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for taonad if --version-string is not set,
# but has different outcomes for taona-qt and taona-cli.
echo "[COPYRIGHT]" > footer.h2m
$TAONAD --version | sed -n '1!p' >> footer.h2m

for cmd in $TAONAD $TAONACLI $TAONATX $TAONAQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${TNAVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${TNAVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
