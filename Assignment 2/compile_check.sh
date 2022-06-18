#! /bin/bash

# May need to run this first: chmod +x compile_check.sh

# Check if zip is installed.
# Cite: https://stackoverflow.com/questions/1298066
if ! command -v zip &> /dev/null; then
    echo "zip is needed to unzip. Install? (y/n): "
    read
    if [ $REPLY = "y" ]; then
        sudo apt-get install zip
    else
        echo "Can't unzip."
        exit
    fi
fi

# List files.
echo
echo "Files in this folder:"
ls
echo

# Check for presence of care-a2.zip
# Cite: https://stackoverflow.com/a/638980
if [ ! -f "care-a2.zip" ]; then
   echo "care-a2.zip  not found."
   echo "Please download and put in this same folder."
   exit
fi

# Unzip submission zip.
echo "Make sure your submission zip file is in this folder."
echo "Enter its name: "
read
unzip $REPLY -d ./Submitted

# Unzip original assignment zip.
unzip care-a2.zip

# Move files into their correct positions.
mv Submitted/Stack.* a2-base/q1/
mv Submitted/eval.cpp a2-base/q3/
mv Submitted/Queue.* a2-base/q4/

# Check compilations.
# Cite: https://stackoverflow.com/a/26675771
if [ ! -f "Submitted/Analysis.pdf" ]; then
    echo "Q2 big-O Analysis.pdf NOT found."
else
    echo "Q2 big-O Analysis.pdf FOUND."
fi

cd a2-base/q1
if g++ -std=c++11 -Wfatal-errors Stack.cpp StkTest.cpp ; then
    echo "Q1 compiled SUCCESSFULLY."
else
    echo "Q1 FAILED to compile."
fi

cd ../q3
if g++ -std=c++11 -Wfatal-errors Scanner.cpp eval.cpp ; then
    echo "Q3 compiled SUCCESSFULLY."
else
    echo "Q3 FAILED to compile."
fi

cd ../q4
if g++ -std=c++11 -Wfatal-errors Queue.cpp main.cpp ; then
    echo "Q4 compiled SUCCESSFULLY."
else
    echo "Q4 FAILED to compile."
fi

cd ../..
exit