for dir in ./Plugex*/
do
    dir=${dir%*/}                           # remove the trailing "/"
    echo "=== Resaving" ${dir##*/} "==="   # print everything after the final "/"
    cd ${dir}
    /home/olivier/JUCE/Projucer --resave *.jucer
    cd ..
done
