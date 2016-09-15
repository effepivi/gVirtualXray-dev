#!/bin/bash

function runTutorial {
    echo "    $2"
    echo ./tutorial_${1}_$2/tutorial_${1}_$2
    if [ -f ./tutorial_${1}_$2/tutorial_${1}_$2 ]
    then
        ./tutorial_${1}_$2/tutorial_${1}_$2 > /dev/null
    else
        echo "    Tutorial $1 using $2 does not exit"
    fi
    
}

for i in `seq -f "%02g" 0 8`;
do
    echo $i
    runTutorial $i "glut"
    runTutorial $i "glfw"
    runTutorial $i "fltk"
    runTutorial $i "qt4"
    runTutorial $i "qt5"
done

