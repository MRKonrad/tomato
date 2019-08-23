# Tomato
A library to calculate parametric maps in MRI. It contains ShMOLLI implementation as in [this article](https://jcmr-online.biomedcentral.com/articles/10.1186/1532-429X-12-69).  
**Github** repository can be found [here](https://github.com/MRKonrad/tomato).  
**Documentation** in doxygen can be found [here](https://mrkonrad.github.io/tomato/html).

| System                                    |     Status                                                         |
|-------------------------------------------|:------------------------------------------------------------------:|
| [DOI][zenodo_link]                        | [![DOI][zenodo_badge]][zenodo_link]                                |
| [OSX/Linux build - Travis][travis_link]   | [![Travis (.org)][travis_badge]][travis_link]                      |
| [Windows build - AppVeyor][appveyor_link] | [![AppVeyor][appveyor_badge]][appveyor_link]                       |
| [Code quality - Codacy][codacy_link]      | [![Codacy Badge][codacy_badge]][codacy_link]                       |
| [Test coverage - Codecov][codecov_link]   | [![codecov][codecov_badge]][codecov_link]                          |
| [Downloads][downloads_link]               | [![GitHub Releases (by Release)][downloads_badge]][downloads_link] |


# Usage
* App - download the latest version of `TomatoExe` from the [Releases][downloads_link] page and follow the youtube tutorial below 
[![Tomato tutorial](https://img.youtube.com/vi/0tzNZNiZh18/0.jpg)](https://www.youtube.com/watch?v=0tzNZNiZh18)
* Lib - download the latest version of `TomatoLib` from the [Releases][downloads_link] page
* Code - if you want to build the code please take a look at the scripts in the `scriptsBuild` and the continuous integrations scripts `.appveyor.yml` and `travis.yml` 

## Tomato and Numerical Recipes

**Important**  
Original ShMOLLI (and based on it Tomato) uses code based on Numerical Recipes book. Due to Numerical Recipes licence I cannot share this part of the code online. Please contact me if you would like to use this part of the code. To make up for this limitation we provide a number of alternative fitting procedures.

# Troubleshooting
* missing msvcp140.dll  
Download https://www.microsoft.com/en-us/download/details.aspx?id=48145 as suggested in https://stackoverflow.com/questions/32998902/msvcp140-dll-missing

# Software Requirements Specification

1. Calculate ShMOLLI with exactly the same results as these from the previous implementation.
2. API for easily choosing
    * a map calculation algorithm,
    * a fitting algorithm,
    * a model function and corresponding energy function.
3. Compatibility with:
    * main operating systems (Windows, MacOS, Linux),
    * C++98 (to include the library in older code).
4. Handling DICOM files.
5. Fast calculation.
6. Easily interfaced to calculation environments MATLAB, IDL and Python.  

# Coding style

* Underscore (`_variable`) means a class member variable.
* C++98 does not allow me to use constructor delegation. I am using methods like `setAllPointersToNull()` to make up for it.
* Order of methods in a class definition:
    * functional (calculation, helpers etc)
    * getters/setters
    * disp()
    * constructors/destructors
* In **copy constructors** copy primitive types, set all pointers to zero.

# Notes

* **Continuous integration** was tested [in this repo](https://github.com/MRKonrad/ContinousIntegrationPlayground).
* **Gitmodules**: after `git clone` run `git submodule update --init --recursive` to get the submodules code.
* It would be nice to follow [CppCoreGuidlines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md) as far as C++98 allows.
* Maybe I can write a script for github releases with [github releases REST api](https://developer.github.com/v3/repos/releases/#create-a-release) and [curl REST api](http://www.codingpedia.org/ama/how-to-test-a-rest-api-from-command-line-with-curl/#12_HEAD_requests)?
* LMFIT - somehow windows does not like the static library, so the dynamic is used

# TODO:

* output SNR maps
* make sure SNR and nAmoebaCalls is the same as in Shmolli
* fix downloadITK_linux_osx
* seperate projects for building tomato dependencies (done for lmfit)
* possibly I could make all FunctionsT1 object static 
* write user-friendly build scripts (and use them in CI)

[zenodo_link]: https://zenodo.org/badge/latestdoi/142749215
[zenodo_badge]: https://img.shields.io/badge/DOI-10.5281%2Fzenodo.1489791-blue.svg?style=flat-square
[travis_link]: https://travis-ci.org/MRKonrad/tomato
[travis_badge]: https://img.shields.io/travis/MRKonrad/tomato/master.svg?style=flat-square
[appveyor_link]: https://ci.appveyor.com/project/MRKonrad/tomato
[appveyor_badge]: https://img.shields.io/appveyor/ci/MRKonrad/tomato/master.svg?style=flat-square
[codacy_link]: https://www.codacy.com/app/MRKonrad/tomato?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=MRKonrad/tomato&amp;utm_campaign=Badge_Grade
[codacy_badge]: https://img.shields.io/codacy/grade/1ca5a2f31ee040cc9258fc5018e51c1e?style=flat-square
[codecov_link]: https://codecov.io/gh/MRKonrad/tomato
[codecov_badge]: https://img.shields.io/codecov/c/github/MRKonrad/tomato.svg?style=flat-square
[downloads_link]: https://github.com/MRKonrad/tomato/releases
[downloads_badge]: https://img.shields.io/github/downloads/MRKonrad/tomato/total.svg?style=flat-square
