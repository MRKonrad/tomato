cd ../thirdParty
mkdir lmfit
cd lmfit
curl -L https://github.com/MRKonrad/build-lmfit/releases/download/lmfit_master/lmfit_master_windows_%PLATFORM%_v120.zip -o lmfit.zip
7z x -r lmfit.zip
del lmfit.zip

echo "lmfit binaries downloaded"