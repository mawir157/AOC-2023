if [ $# -gt 0 ]
then
  day=$(printf "%02d" $1)
else
  day=-1
fi

BUILDTAGS=()

if [ $day -gt 0 ]
then
	if test -f Day$day/Day$day.go;
	then
    BUILDTAGS+=("d$day")
	else
		echo "Day " $day " does not exist"
	fi
else
	missing=""
	for i in $(seq -f "%02g" 1 25)
	do
		if test -f Day$i/Day$i.go;
		then
      BUILDTAGS+=("d$i")
		else
			if [ "$missing" = "" ]
			then
				missing=$i
			else
				missing=$missing","$i
			fi
		fi
	done
	if [ "$missing" != "" ]
	then
		echo "Missing days = ["$missing"]"
	fi
fi

go build -tags="${BUILDTAGS[*]}" -o aoc2023 . 
./aoc2023
rm -rf aoc2023
