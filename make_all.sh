arg=$1

if [[ "$OSTYPE" == "linux-gnu" ]]; then
	if [ $# -eq 1 ]; then
	    if [ ${arg} == "clean" ]; then
	        for dir in ./Plugex*/
	        do
	            cd ${dir}/Builds/LinuxMakefile/
	            make clean CONFIG=Release
	            cd ../../..
	        done
	        exit 0
	    fi
	fi


	for dir in ./Plugex*/
	do
	    dir=${dir%*/}                           # remove the trailing "/"
	    echo "=== Compiling" ${dir##*/} "==="   # print everything after the final "/"
	    cd ${dir}/Builds/LinuxMakefile/
	    make CONFIG=Release
	    cp build/*.so ~/.vst/
	    cd ../../..
	done

elif [[ "$OSTYPE" == "darwin"* ]]; then
	if [ $# -eq 1 ]; then
	    if [ ${arg} == "clean" ]; then
	        for dir in ./Plugex*/
	        do
	            cd ${dir}/Builds/MacOSX/
	            xcodebuild -quiet -configuration Release clean
	            cd ../../..
	        done
	        exit 0
	    fi
	fi


	for dir in ./Plugex*/
	do
	    dir=${dir%*/}                           # remove the trailing "/"
	    echo "=== Compiling" ${dir##*/} "==="   # print everything after the final "/"
	    cd ${dir}/Builds/MacOSX/
	    xcodebuild -quiet -configuration Release build
        cp -r build/Release/*.component ../../../MacOS_x64_binaries/Components
        cp -r build/Release/*.vst ../../../MacOS_x64_binaries/VST
        cp -r build/Release/*.vst3 ../../../MacOS_x64_binaries/VST3
	    cd ../../..
	done

fi