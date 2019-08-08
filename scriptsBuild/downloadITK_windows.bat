cd ../..
mkdir ITK_install
cd ITK_install
curl -L https://github.com/MRKonrad/build-itk/releases/download/itk_v4.13.2/itk_v4.13.2_windows_%PLATFORM%_%toolset%.zip -o itk_install.zip
7z x -r itk_install.zip
del itk_install.zip

echo "ITK binaries downloaded"
