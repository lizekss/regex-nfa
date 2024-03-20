#! /bin/bash
IN_DIR="P2/In (public)"
OUT_DIR="P2/Out (public)"

declare -i correct=0
g++ run.cpp
for i in {00..19}
    do  
        if [ "$i" -lt "10" ];
        then
            res=$(cat "${IN_DIR}/in0${i}.txt" | ./a.out)
            ans=$(cat "${OUT_DIR}/out0${i}.txt")
        else 
            res=$(cat "${IN_DIR}/in${i}.txt" | ./a.out)
            ans=$(cat "${OUT_DIR}/out${i}.txt")
        fi
        echo "Your answer:    ${res}"
        echo "Correct Answer: ${ans}"
        if [ "$res" ==  "$ans" ]; 
        then
            echo "1/1"
            ((correct=correct+1))
        else
            echo "0/1"
        fi
        echo
    done    
echo "${correct}/20"