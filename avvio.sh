#!/bin/bash
if [ "$1" != "NORMALE" ]; then
    if [ "$1" != "ARTIFICIALE" ]; then
    	echo "MODALITÀ SELEZIONATA NON RICONOSCIUTA:SCEGLIERE TRA NORMALE E ARTIFICIALE"
   	    exit -1
   	fi
fi



x-terminal-emulator -e bin/in.out $1 & bin/out.out 


