#!/bin/bash

for i in slide_*.jpg; do convert ${i} -resize $1x$2 ${i}; done

