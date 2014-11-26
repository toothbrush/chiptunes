#!/bin/sh
# This feeds stdin to a raw audio device. to be used like so:
#
# $ cat /dev/random | ./thisscript
#
#.. obviously you should be more imaginative than /dev/random.

cat | padsp tee /dev/dsp > /dev/null
