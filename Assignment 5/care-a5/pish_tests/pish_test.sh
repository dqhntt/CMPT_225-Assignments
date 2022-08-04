#!/bin/bash
# Very helpful: https://www.shellcheck.net/

# May need to run this first: chmod +x pish_test.sh

# Cite: https://stackoverflow.com/a/9906113
echo -n "Did you do extension 1: Short-Circuit Evaluation? [y/n]: "
read -r ext1
if [ $ext1 = "y" ]; then
    find *.ext1.pish -maxdepth 1 -type f
fi

echo -n "Did you do extension 2: Exception Handling? [y/n]: "
read -r ext2
if [ $ext2 = "y" ]; then
    find *.ext2.pish -maxdepth 1 -type f
fi

echo -n "Did you do extension 3: break ;? [y/n]: "
read -r ext3
if [ $ext3 = "y" ]; then
    find *.ext3.pish -maxdepth 1 -type f
fi

echo -n "Did you do extension 4: if/elif/else? [y/n]: "
read -r ext4
if [ $ext4 = "y" ]; then
    find *.ext4.pish -maxdepth 1 -type f
fi

echo "If the below is empty, all outputs are as expected."
echo

# Patterns that match no files expand to zero arguments.
# Cite: https://stackoverflow.com/a/26255830
shopt -s nullglob

# Check if python3 is installed.
# Cite: https://stackoverflow.com/questions/1298066
command -v python3 &> /dev/null
py3=$?

# Get outputs from both stderr and stdout.
for PISH in *.pish
do
    ./pish < $PISH &> $PISH.got
    ./testParse < $PISH &> $PISH.parsed
    ./transform < $PISH.parsed &> $PISH.py
    if [ $py3 ] && [ ! -f $PISH.expected ]; then
        python3 $PISH.py &> $PISH.py.expected
    fi
done

# Try to fix line endings.
for FILE in *
do
    sed -i -e 's/\r$//' $FILE
done

# Check for differences.

# 1st arg = PISH
diff_pish () {
    if [ -f $1.expected ]; then
        if ! diff -y $1.got $1.expected &> $1.diff; then
            echo "WARNING: DIFF detected: $1"
        fi
    elif [ -f $1.py.expected ]; then
        if ! diff -y $1.got $1.py.expected &> $1.py.diff; then
            echo "WARNING: Py3 DIFF detected: $1"
        fi
    else
        echo "WARNING: No expected output file for $1"
    fi
}

# Regular pish.
for PISH in *.reg.pish
do
    diff_pish $PISH
done

if [ $ext1 = "y" ]; then
    for PISH in *.ext1.pish
    do
        diff_pish $PISH
    done
fi

if [ $ext2 = "y" ]; then
    for PISH in *.ext2.pish
    do
        diff_pish $PISH
    done
fi

if [ $ext3 = "y" ]; then
    for PISH in *.ext3.pish
    do
        diff_pish $PISH
    done
fi

if [ $ext4 = "y" ]; then
    for PISH in *.ext4.pish
    do
        diff_pish $PISH
    done
fi

exit
