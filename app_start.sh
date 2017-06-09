#!/bin/bash
source ./build_package.sh
pids=()
echo "Choose the application configuration file"
read file

while read name
do
  
  if [ "$name" = ">" ]; then
          break
  else
      echo $BABS
	  echo "STARTING: " $name;
          xterm -e $BABS/bin/$name &
          let temp=$!
          pids+=($temp)
	  
  fi
 
done < $file.md;

echo ${#pids[*]} ' modules are running'


echo 'type q to exit program, do NOT do it during flight'
while true
do

	read q
	if [ "$q" = "q" ]; then
		  
		for i in ${pids[*]}
		do

			echo killing process $i	
			kill -INT $i

		done
		exit

	else
		echo 'invalid command'
	fi
done
