#!/bin/bash

#HOWTO:
# Parameters:
# 1: Presentation File
# 2: Number of Slides
# 3: Sampling density (ca 100-1000)
# 4&5: Output Resolution

lastslide=5

pdfseparate $1 -l $2 slide_%d.pdf


w=$4
h=$5

h3rd=$(($h / 3))
halfW=$(($w / 2))

for i in slide_*.pdf; do convert -density $3 ${i} -resize ${w}x${h} ${i%pdf}jpg; done
rm slide_*.pdf

#mv slide_*.jpg ../slides/


