#!/bin/bash

# TODO: download itk binaries from https://github.com/MRKonrad/build-itk. Problem:
# /usr/bin/ld: /home/travis/build/MRKonrad/ITK_install/lib/libitksys-4.13.a(SystemTools.cxx.o): unrecognized relocation (0x2a) in section `.text'
# /usr/bin/ld: final link failed: Bad value

export mysystem=`uname`

if [ "${mysystem}" == "Darwin" ]; then

  export mysystem="osx";
fi

echo ${mysystem}
cd ../..

#mkdir ITK_install
#cd ITK_install
#curl -L -O https://github.com/MRKonrad/build-itk/releases/download/itk_v4.13.2/itk_v4.13.2_${mysystem}_g++_CXX98.zip
#unzip -a -q itk_v4.13.2_${mysystem}_g++_CXX98.zip
#rm itk_v4.13.2_${mysystem}_g++_CXX98.zip

curl -L -O http://mrkonrad.github.io/MRKonrad/files/ITK_built/ITK413_${mysystem}_install.zip
unzip -a -q ITK413_${mysystem}_install.zip
rm ITK413_${mysystem}_install.zip

echo "ITK binaries downloaded"
