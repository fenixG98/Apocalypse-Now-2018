 #!/bin/bash          
STR=0
j=0
while [ $j -lt 100 ]; do

	
	./Game "DZoZ_SedrK17" "Dummy" "Dummy" "Dummy" "-s" $j "-i" "default.cnf" |& grep "info: player(s) DZoZ_SedrK17 got top score"  > /dev/null && let STR+=1
	let j+=1
done

j=0
while [ $j -lt 100 ]; do

	
	./Game "Dummy" "DZoZ_SedrK17" "Dummy" "Dummy" "-s" $j "-i" "default.cnf" |& grep "info: player(s) DZoZ_SedrK17 got top score"  > /dev/null && let STR+=1
	let j+=1
done
j=0
while [ $j -lt 100 ]; do

	
	./Game "Dummy" "Dummy" "DZoZ_SedrK17" "Dummy" "-s" $j "-i" "default.cnf" |& grep "info: player(s) DZoZ_SedrK17 got top score"  > /dev/null && let STR+=1
	let j+=1
done
j=0
while [ $j -lt 100 ]; do

	
	./Game "Dummy" "Dummy" "Dummy" "DZoZ_SedrK17" "-s" $j "-i" "default.cnf" |& grep "info: player(s) DZoZ_SedrK17 got top score"  > /dev/null && let STR+=1
	let j+=1
done


echo $STR
