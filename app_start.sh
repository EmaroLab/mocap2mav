#!/bin/bash

echo "Choose the application file"
read file

while read name
do
  
  if [ "$name" = ">" ]; then
          break
  else
          
	  echo "STARTING: " $name;
          xterm -e ./build/bin/$name &
           
  fi
 
done < $file.md;
exit;
