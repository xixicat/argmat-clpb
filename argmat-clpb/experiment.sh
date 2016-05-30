#!/bin/bash
datasets=(
#'../datasets/kleinberg/9/'
#'../datasets/kleinberg/16/'
#'../datasets/kleinberg/25/'
#'../datasets/kleinberg/36/'
#'../datasets/test_cases2/apx/'
#'../datasets/kleinberg/49/'
#'../datasets/kleinberg/64/'
../datasets/iccma15_testcases1/tgf/
)
# file type
FTYPE='tgf'
#FTYPE='tgf'

# file extension
F_EXT='*.tgf'

#set time cmd to track the time that the cmd has been executing
TIME_CMD='/usr/bin/time -v'
#TIME_CMD='time'  # bash time cmd

PROGRAM='./argmat-clpb.out'

TIME_LIMIT='60m'


# set TASK
#TASK_SET=('EE-ST1' 'EE-ST2' 'EE-CF1' 'EE-CF2' 'EE-CF3' 'EE-CO2' 'EE-CO3' 'EE-CO4' 'EE-CO5')
# TASK_SET=('EE-CO1' 'EE-AD1' 'EE-AD2' 'EE-AD3' 'EE-AD4' 'EE-AD5')
TASK_SET=('EE-ST' 'EE-CO')
for TASK in ${TASK_SET[@]}; do
	# set the directory to save experimental results
	DAY='20160528'
	RES_PATH='./res/clpb3/'
	OPATH=${RES_PATH}${TASK}'_'${DAY}'/sols/'  # to store program outputs
	TPATH=${RES_PATH}${TASK}'_'${DAY}'/time/'  # to stroe executing time information

	# create paths
	mkdir -p ${OPATH}
	mkdir -p ${TPATH}

	# The result bash file
	BASH_FILE=${TASK}'_'${DAY}'-clpb3.sh'

	rm -f ${BASH_FILE}

	echo '#!/bin/bash' >> ${BASH_FILE}
	for var in ${datasets[@]}; do
		echo "Reading test sets from " $var
		inFiles=($(find ${var} -name ${F_EXT} -print))
		for inFile in ${inFiles[@]}; do
			fname=$(basename ${inFile})
			fdir=$(dirname ${inFile})
			outFile=${OPATH}${TASK}'_'${fname}'.sols'
			timeFile=${TPATH}${TASK}'_'${fname}'.time'

			#run_cmd='timeout '${TIME_LIMIT}' '${TIME_CMD}' '${PROGRAM}' -f '${inFile}' -fo '${FTYPE}' -p '${TASK}' -o '${outFile}' 2>'${timeFile}
			run_cmd='timeout '${TIME_LIMIT}' '${TIME_CMD}' '${PROGRAM}' -f '${inFile}' -fo '${FTYPE}' -p '${TASK}' > '${outFile}' 2>'${timeFile}
			#run_cmd=${TIME_CMD}' '${PROGRAM}' -f '${inFile}' -fo '${FTYPE}' -p '${TASK}' > '${outFile}' 2>'${timeFile}
			echo 'echo "RUN the case ['${fname}'] with task ['${TASK}']"' >> ${BASH_FILE}
			echo 'echo "  DONE!"' >> ${BASH_FILE}
			echo ${run_cmd} >> ${BASH_FILE}
			echo >> ${BASH_FILE}
		done
	done

	echo 'echo "  All case are completed!"' >> ${BASH_FILE}

	echo 'Executing '${TASK}' ...'
	bash ${BASH_FILE}
	echo ${TASK}' is completed!'
	mv ${BASH_FILE} ${RES_PATH}${TASK}'_'${DAY}'/'
done


