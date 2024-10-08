#!/bin/bash
#
# Serches thru all code to see if any source file or header contains any of the following banned words:
# TODO, FIXME
#

BANNED_WORDS="TODO|FIXME"

# First find all source files (.c) and all header files (.h) with path in quotes
RESULT=$(find . -name '*.[ch]' -print0 | xargs -0 cat | grep -Ei -- "$BANNED_WORDS")

if [ "$RESULT" != "" ]
then
    echo some files contains banned words
    find . -name '*.[ch]' -print0 | xargs -0 \
    awk "BEGIN {
        }
        {
            name=FILENAME
            IGNORECASE=1
		    if (\$0 ~ /$BANNED_WORDS/) {
                printf \"%s:%s \t %s\n\", name, FNR, \$0;
            }
        }
    "
    exit 1;
fi
exit 0;