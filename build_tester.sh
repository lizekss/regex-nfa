#! /bin/bash
IN_DIR="P2/In (public)"
OUT_DIR="P2/Out (public)"
declare -i correct=0
for i in {00..19}
    do 
        if [ "$i" -lt "10" ];
        then
            INFILEPATH="${IN_DIR}/in0${i}.txt"
            OUTFILEPATH="${OUT_DIR}/out0${i}.txt"
            REGPATH="${IN_DIR}/reg0${i}.txt"
        else 
            INFILEPATH="${IN_DIR}/in${i}.txt"
            OUTFILEPATH="${OUT_DIR}/out${i}.txt"
            REGPATH="${IN_DIR}/reg${i}.txt"
        fi
        g++ build.cpp
        head -1 "$INFILEPATH" > tmp.txt
        cat "${REGPATH}" | ./a.out >> tmp.txt
        g++ run.cpp
        res=$(cat tmp.txt | ./a.out)
        ans=$(cat "${OUTFILEPATH}")
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