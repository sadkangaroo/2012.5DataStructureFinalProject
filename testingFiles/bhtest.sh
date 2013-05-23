#!/bin/bash

echo -n "Your program name: "
read program_name
echo -n "Begin Value: "
read beginValue
echo -n "End Value: "
read endValue

echo ""
echo "################################"
echo "         Testing starts...      "
echo ""

num=0;
for ((N=$beginValue; N<=$endValue; N=N*10))
do
	((num++))
	echo "TEST CASE $num"
        echo "size: "$N
	echo -n "My Cpp STL: "
	(time -p ./$program_name $N) 2>&1 | grep real | sed 's/real/TIME/'
	echo -n "Offical Java STL: "
	(time -p java $program_name $N) 2>&1 | grep real | sed 's/real/TIME/'
	echo ""
done

echo "         All tests done.      "
echo "################################"
echo ""
