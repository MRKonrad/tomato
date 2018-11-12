# Tomato
A library to calculate parametric maps in MRI. It contains ShMOLLI implementation as in [this article](https://jcmr-online.biomedcentral.com/articles/10.1186/1532-429X-12-69).

| OSX/Linux - Travis | Windows - AppVeyor |  Test coverage - Codecov | Downlads |
|:-------------:|:-------------:|:-------------:| :-------------:| 
[![Travis (.org)][travis_badge]](travis_link) | [![AppVeyor][appveyor_badge]](appveyor_link)| [![codecov][codecov_badge]](codecov_link)| ![GitHub Releases (by Release)][downloads_badge]


## Documentation

**Documentation** in doxygen can be found [here](https://mrkonrad.github.io/tomato/html).

## Tomato and Numerical Recipes

**Important**  
Original ShMOLLI (and based on it Tomato) uses code based on Numerical Recipes book. Due to Numerical Recipes licence I cannot share this part of the code online. Please contact me if you would like to use this part of the code. To make up for this limitation we provide a number of alternative fitting procedures. 

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

[travis_badge]: https://img.shields.io/travis/MRKonrad/tomato.svg?style=for-the-badge
[travis_link]: https://travis-ci.org/MRKonrad/tomato
[appveyor_badge]: https://img.shields.io/appveyor/ci/MRKonrad/tomato.svg?style=for-the-badge
[appveyor_link]: https://ci.appveyor.com/project/MRKonrad/tomato
[codecov_badge]: https://img.shields.io/codecov/c/github/MRKonrad/tomato.svg?style=for-the-badge
[codecov_link]: https://codecov.io/gh/MRKonrad/tomato
[downloads_badge]: https://img.shields.io/github/downloads/MRKonrad/tomato/v0.1/total.svg?style=for-the-badge

