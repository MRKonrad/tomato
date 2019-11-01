#!/usr/bin/env bash

# In this script a sample use of TOMATO software is presented
# The TOMATO code can be found here: https://github.com/MRKonrad/tomato
# Binaries used here have been build in continous integration systems, their scripts can be found here:
# Travis (Linux and Osx): https://github.com/MRKonrad/tomato/blob/master/.travis.yml
# Appveyor (Windows): https://github.com/MRKonrad/tomato/blob/master/.appveyor.yml
# The code has not been built using Codeocean to avoid redundancy with the builds listed above

echo ""
echo "### Get TOMATO executable"
curl -L -O https://github.com/MRKonrad/tomato/releases/download/v0.4/TomatoExe_v0.4_linux.zip
unzip -a -q TomatoExe_v0.4_linux.zip

echo ""
echo "### Get some sample data"
mkdir testData
cd testData
curl -L -O https://github.com/MRKonrad/tomato/releases/download/v0.4/testData.zip
unzip -a -q testData.zip
cd ..

echo ""
echo "### Check the contents of the main folder"
ls -la

echo ""
echo "### Check the contents of the data folder"
ls -la testData

echo ""
echo "### Run an example"
echo "### the configuration file (tomatoConfig_Shmolli_inputFilePaths.yaml) contains all the adjustable calculation parameters"
./TomatoExe testData/tomatoConfig_Shmolli_inputFilePaths.yaml

echo ""
echo "### Check the contents of the output folder with T1 maps"
ls -la testData/tomatoOutput_Shmolli/map

echo ""
echo "### Check the contents of the output folder with other maps"
ls -la testData/tomatoOutput_Shmolli/fitparams