#!/bin/bash

ENABLE_MAKE_CLEAN=1
ENABLE_MAKE=1
GET_ASM=1
ENABLE_RUN=1

CC="g++"
EXE_FILE="main.adx"
C_FILE="test.cpp"
MAP_FILE="main.map"
LFLAGS="-Wl,-Map=${MAP_FILE},--cref "
LFLAGS="-lfftw3 -lm ${LFLAGS}"
LFLAGS="`pkg-config --cflags --libs opencv4` ${LFLAGS}"
#LFLAGS="-lopencv_core452 -lopencv_imgcodecs452 -lopencv_imgproc452 -lopencv_calib3d452 -lopencv_dnn452 -lopencv_features2d452 -lopencv_flann452 -lopencv_gapi452 -lopencv_highgui452 -lopencv_ml452 -lopencv_objdetect452 -lopencv_photo452 -lopencv_stitching452 -lopencv_video452 -lopencv_videoio452 ${LFLAGS}"
LFLAGS="-Wl,--gc-sections ${LFLAGS}"
CFLAGS="-W -Wall -O0 -g3 ${LFLAGS}"
CFLAGS="-ffunction-sections -fdata-sections ${CFLAGS}"
CFLAGS=" ${CFLAGS}"

#generate ENV_FILE
ENV_FILE="NLIB_CROSS"

echo "CC=${CC}">${ENV_FILE}
echo "EXE_FILE=${EXE_FILE}">>${ENV_FILE}
echo "C_FILE=${C_FILE}">>${ENV_FILE}
echo "CFLAGS=${CFLAGS}">>${ENV_FILE}

if [ ${ENABLE_MAKE_CLEAN} -eq 1 ]; then
	make clean
fi

chmod 700 ${C_FILE}

if [ ${ENABLE_MAKE} -eq 1 ]; then
	make 1>make.log 2>make_warning.log
fi

#rep -inIr "\.text          " ${MAP_FILE}
#grep -inIr "\.text          " ${MAP_FILE} | awk '$0 ~/tmp/ {print}'
#grep -inIr "\.text          " ${MAP_FILE} | awk '{ printf "text =  %s\n",$3, $4} '


if [ ${GET_ASM} -eq 1 ]; then
	objdump -S ${EXE_FILE} > asm.log
	objdump -t ${EXE_FILE} > symbol.log
fi

if [ ${ENABLE_RUN} -eq 1 ]; then
	echo "below is result of ${EXE_FILE} :"
	./${EXE_FILE}
fi
