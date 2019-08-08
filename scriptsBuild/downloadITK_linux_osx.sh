#!/bin/bash

#TODO: do the same for windows

export mysystem=`uname`

if [ "${mysystem}" == "Darwin" ]; then

  export mysystem="osx";
fi

echo ${mysystem}
cd ../..
mkdir ITK_install
cd ITK_install
curl -L -O https://github.com/MRKonrad/build-itk/releases/download/itk_v4.13.2/itk_v4.13.2_${mysystem}_g++_CXX98.zip
unzip -a -q itk_v4.13.2_${mysystem}_.zip
rm itk_v4.13.2_${mysystem}_.zip

echo "ITK binaries downloaded"
