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
curl -L https://github.com/MRKonrad/build-lmfit/releases/download/lmfit_master/lmfit_master_${mysystem}_g++_CXX98.zip -o lmfit.zip
unzip -a -q lmfit.zip
rm lmfit.zip

echo "lmfit binaries downloaded"