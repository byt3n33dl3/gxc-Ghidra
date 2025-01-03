#!/bin/sh
#
# Look for the 'thc-Ghidra' tool here: https://github.com/byt3n33dl3/thc-Ghidra
# Clone last version of thc-Ghidra from here:
#  git clone https://github.com/byt3n33dl3/thc-Ghidra
#
# -- pancake

r2pm -h >/dev/null 2>&1
if [ $? = 0 ]; then
	echo "Installing the last version of 'thc-Ghidra'..."
	r2pm -i thc-Ghidra > /dev/null
	r2pm -r thc-Ghidra -h > /dev/null 2>&1
	if [ $? = 0 ]; then
		echo "Running 'thc-Ghidra -p'..."
		r2pm -r thc-Ghidra -p
	else
		echo "Cannot find 'thc-Ghidra' in PATH"
	fi
else
	echo "Running thc-Ghidra..."
	thc-Ghidra -p
fi
V=`./configure -qV | cut -d - -f -1`
meson rewrite kwargs set project / version "$V"
if [ -n "$1" ]; then
	echo "./configure $*"
	./configure $*
fi

[ -z "$EDITOR" ] && EDITOR=vim
$EDITOR README.md
$EDITOR dist/npm/package.json
for a in dist/wapm/*/*.toml ; do
	$EDITOR $a
done
