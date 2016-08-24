#!/bin/bash
if [ $(uname -s) = "Linux" ]
    then
        echo export LD_LIBRARY_PATH=${1}
    elif [ $(uname -s) = "Darwin" ]
    then
        echo export DYLD_LIBRARY_PATH=${1}
    else
        echo "No match"
fi
        
shift
while [ $# -gt 0 ]
    do
    if [ $(uname -s) = "Linux" ]
        then
            echo export LD_LIBRARY_PATH='${LD_LIBRARY_PATH}:'${1}
        elif [ $(uname -s) = "Darwin" ]
        then
            echo export DYLD_LIBRARY_PATH='${DYLD_LIBRARY_PATH:'${1}
        else
            echo "No match"
    fi
    shift
done
