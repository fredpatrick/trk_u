#!/bin/sh
echo '#! /bin/csh -f'     
if [ $(uname -s) -eq Linux ]
    then
        echo export LD_LIBRARY_PATH=${1}
    elif [ $(uname -s) -eq Darwin ]
        echo export DYLD_LIBRARY_PATH=${1}
    else
        echo "No match"
fi
        
shift
while [ $# -gt 0 ]
    do
    if [ $(uname -s) -eq Linux ]
        then
            echo export LD_LIBRARY_PATH='${LD_LIBRARY_PATH}:'${1}
        elif [ $(uname -s) -eq Darwin ]
            echo export DYLD_LIBRARY_PATH='${DYLD_LIBRARY_PATH:'${1}
        else
            echo "No match"
    fi
    shift
done
