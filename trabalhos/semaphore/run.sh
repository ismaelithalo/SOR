#!/bin/bash
for i in {1..10}; do
	#python test.py | grep 'sum = 9'
	./prod_cons &
done
