#!/bin/sh
touch ~/.gdbinit
if [ -n "$(grep 'set auto-load safe-path /' ~/.gdbinit)" ]
then echo ""
else echo "set auto-load safe-path /" >> ~/.gdbinit
fi
