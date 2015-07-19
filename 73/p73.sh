#!/bin/bash
i=4
while [  $i -lt 100 ]; do
	./p73 $i
	let i=i+1
done
