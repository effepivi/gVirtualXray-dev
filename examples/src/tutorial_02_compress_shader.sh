#!/usr/bin/sh

gzip --best -c tutorial_02.frag > tutorial_02.frag.gz
gzip --best -c tutorial_02.vert > tutorial_02.vert.gz

bin2c --name g_tutorial_02_frag tutorial_02.frag.gz > tutorial_02.frag.h
bin2c --name g_tutorial_02_vert tutorial_02.vert.gz > tutorial_02.vert.h

