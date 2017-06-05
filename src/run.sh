#!/bin/bash

#this is a wrapper script that runs both make and built app automatically

make

if [ -f "../bin/todoApp" ];
then
	./../bin/todoApp -d ../bin/data.txt
fi
