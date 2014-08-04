#!/bin/bash
COMPILE=0  # 1 -> compile first
if [ $COMPILE -eq 1 ];then
    make
fi

./bin/jpscore  --inifile=demos/scenario_1_corridor/corridor_ini.xml 
