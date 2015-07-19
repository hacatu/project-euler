#!/bin/bash
for i in `seq 20 88`;
do
	mkdir $i
	mv p$i* $i
done
