#!/bin/sh

g++ ${1}.cpp -I ~/CODE/Libraries/Boost/boost_1_58_0/ -o ../bin/${1}
../bin/${1}
cd ../Output
for i in *.gv; do
	j="$(echo $i | sed -e 's/\(.gv\)*$//g')"
	neato -Tbmp ${j}.gv -o ${j}_neato.bmp
	#fdp -Tbmp ${j}.gv -o ${j}_fdp.bmp
	#patchwork -Tbmp ${j}.gv -o ${j}_patchwork.bmp
	#dot -Tbmp ${j}.gv -o ${j}_dot.bmp
	#sfdp -Tbmp ${j}.gv -o ${j}_sfdp.bmp
	#circo -Tbmp ${j}.gv -o ${j}_circo.bmp
	#osage -Tbmp ${j}.gv -o ${j}_osage.bmp
	#twopi -Tbmp ${j}.gv -o ${j}_twopi.bmp
done
cd ../src
