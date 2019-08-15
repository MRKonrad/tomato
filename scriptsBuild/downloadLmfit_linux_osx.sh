#!/bin/bash

mysystem=$(uname)
export mysystem

if [ "${mysystem}" == "Darwin" ]
then
  export mysystem="osx";
fi

if [ "${mysystem}" == "Linux" ]
then
  export mysystem="linux";
fi

echo ${mysystem}
cd ../thirdParty

mkdir lmfit
cd lmfit
curl -L -O https://github.com/MRKonrad/build-lmfit/releases/download/lmfit_master/lmfit_master_${mysystem}_g++_CXX98.zip
unzip -a -q lmfit_master_${mysystem}_g++_CXX98.zip
rm lmfit_master_${mysystem}_g++_CXX98.zip

echo "lmfit binaries downloaded"