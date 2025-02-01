#!/bin/bash
echo Writer shell file

#checking if 2 inputs are given or not
if [[ $# -ne 2 ]]; then
	echo "ERROR : WRONG INPUT"
	exit 1
fi

GIVEN_PATH="$1"
CONTENT="$2"

#Extracting the Path and removing the file name
echo "$GIVEN_PATH"
echo "Removing the last element"
NEW_PATH=$(dirname "$GIVEN_PATH")
echo "Directory $NEW_PATH"

echo " "
#Checking if directory exists
if [ ! -d "$NEW_PATH" ]; then
	echo "$NEW_PATH does not exist"
	exit 1
fi
#Extracting the file name
FILE_NAME=$(basename "$GIVEN_PATH")
echo "Filename $FILE_NAME"
echo ""

#Going to a new path/directory
cd $NEW_PATH
echo "In a new path"
echo " "

#Generating a file with extracted file name and specified content
echo "New file generated"
echo "$CONTENT" > $FILE_NAME

#Checking if file is properly generated or not in desired directory
ls | grep $FILE_NAME

#Reading the file back
echo "Written the content"
cat $FILE_NAME

