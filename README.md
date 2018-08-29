# OxShmolli2
A library to calculate parametric maps in MRI. It contains ShMOLLI implementation as in [this article](https://jcmr-online.biomedcentral.com/articles/10.1186/1532-429X-12-69).

| Travis (OSX/Linux) | AppVeyor (Windows) | Coveralls (test coverage) |
|:-------------:|:-------------:|:-------------:|
[![Travis (.org)][travis_badge]][travis_link] | [![AppVeyor][appveyor_badge]][appveyor_link]| [![Coveralls][coveralls_badge]][coveralls_link]


## Documentation
**Documentation** in doxygen can be found [here](https://mrkonrad.github.io/OxShmolli2/html).

## OxShmolli2 and Numerical Recipes
**Important**  
Original ShMOLLI (and based on it OxShmolli2) uses code based on Numerical Recipes book. Due to Numerical Recipes licence I cannot share this part of the code online. Please contact me if you would like to use this part of the code. To make up for this limitation we provide a number of alternative fitting procedures. 

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

[travis_badge]: https://img.shields.io/travis/MRKonrad/OxShmolli2.svg?style=for-the-badge
[travis_link]: https://travis-ci.org/MRKonrad/OxShmolli2
[appveyor_badge]: https://img.shields.io/appveyor/ci/MRKonrad/OxShmolli2.svg?style=for-the-badge
[appveyor_link]: https://ci.appveyor.com/project/MRKonrad/oxshmolli2/
[coveralls_badge]: https://img.shields.io/coveralls/github/MRKonrad/OxShmolli2.svg?style=for-the-badge
[coveralls_link]: https://coveralls.io/github/MRKonrad/OxShmolli2
