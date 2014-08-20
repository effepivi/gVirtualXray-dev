#!/usr/bin/sh

gzip --best -c tutorial_03.frag > tutorial_03.frag.gz
gzip --best -c tutorial_03.vert > tutorial_03.vert.gz

bin2c --name g_tutorial_03_frag tutorial_03.frag.gz > tutorial_03.frag.h
bin2c --name g_tutorial_03_vert tutorial_03.vert.gz > tutorial_03.vert.h

