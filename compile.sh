BUILD_DIR=./build
CSSMH_DIR=./cssmh
CSSMH_SRC=$(find $CSSMH_DIR -type f -name "*.cpp" -exec echo "{} " \;)
CSSMH_OUT=libcssmh.so
HL2SDK_DIR=$CSSMH_DIR/hl2sdk
HL2SDK_INC=$(find $HL2SDK_DIR -type d -exec echo "-I{} " \;)

if [ -d $BUILD_DIR ]; then
	rm -rf $BUILD_DIR
fi

mkdir $BUILD_DIR

gcc -std=gnu++11 -o $BUILD_DIR/$CSSMH_OUT -g -fpermissive -shared -fPIC -m32 $CSSMH_SRC $HL2SDK_INC -ldl -lSDL2 -lstdc++ -pthread -static-libstdc++ -Wl,--no-undefined,--no-allow-shlib-undefined
