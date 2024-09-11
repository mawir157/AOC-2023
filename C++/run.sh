if [ $# -gt 0 ]
then
  day=$(printf "%02d" $1)
else
  day=-1
fi

if test -f aoc2023;
then
	rm -rf aoc2023
fi

if [ $day -gt 0 ]
then
	if test -f Day$day.cpp;
	then
		echo "compiling single day..."
		g++ *.cpp -O2 -o aoc2023 -std=c++17 -DDAY$day -Wall -Wextra -Wunused-variable
		echo "done."
		./aoc2023
		rm -rf aoc2023
	else
		echo "Day" $day "does not exist"
	fi
else
	missing=""
	COMPILERSTRING=" "
	for i in $(seq -f "%02g" 1 25)
	do
		if test -f Day$i.cpp;
		then
			COMPILERSTRING+="-DDAY$i "
		else
			if [ "$missing" = "" ]
			then
				missing=$i
			else
				missing=$missing","$i
			fi
		fi
	done
	echo "compiling all days..."
	g++ *.cpp -O2 -o aoc2023 -std=c++17 $COMPILERSTRING
	echo "done."
	./aoc2023
	rm -rf aoc2023
	if [ "$missing" != "" ]
	then
		echo "Missing days = ["$missing"]"
	fi
fi
