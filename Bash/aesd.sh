#!/bin/bash
echo Welcome to ifelif program

# spacing should be like this only
# ${1,,} make sure to convert input in lower case, basically input is case insensitive
if [ ${1,,} = bhakti ]; then
	echo "You are the Boss ma'am ....."
elif [ ${1,,} = hey ]; then
	echo "Yes, hey ?"
elif [ ${1,,} = help ]; then
	echo "Yes, What happened ?"
else 
	echo "You are not boss of me"
# declaration of end of ifelif loop, just if spelled backwords
fi


echo "Original: $1"
# converts into lowercase
echo "Lowercase: ${1,,}"
# converts into uppercase
echo "Uppercase: ${1^^}"

target_path="/home/bakri/Work/Learnings"

cd "$target_path"

pwd

filesdir=$1
searchstr=$2

cd "$filesdir"
echo "Current Directory"
pwd
echo " "


number_of_files=0
number_of_occurance=0
for i in *;
do
	#checking if its a regular file
	if [[ -f "$i" ]];
	then
		if [[ "$i" == *"$searchstr"* ]];
		then
			echo "Found $searchstr"
			number_of_occurance=$((number_of_occurance+1));
		else
			echo " "
		fi
	else
		echo "Not a file"
	fi
	number_of_files=$((number_of_files+1));

done

echo "Total number of occurance : $number_of_occurance"
echo "Total number of file : $number_of_files"
echo "program end"





