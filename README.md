# OxShmolli2
A library to calculate parametric maps in MRI. It contains ShMOLLI implementation as in [this article](https://jcmr-online.biomedcentral.com/articles/10.1186/1532-429X-12-69).

| Travis (OSX/Linux) | AppVeyor (Windows) | Coveralls (test coverage) |
|:-------------:|:-------------:|:-------------:|
[![Travis (.org)][travis_badge]][travis_link] | [![AppVeyor][appveyor_badge]][appveyor_link]| [![Coveralls][coveralls_badge]][coveralls_link]

# OxShmolli2 and Numerical Recipes
**IMPORTANT**  
Original ShMOLLI (and based on it OxShmolli2) uses code based on Numerical Recipes book. Due to Numerical Recipes licence I cannot share this part of the code online. Please contact me if you would like to use this part of the code. To make up for this limitation we provide a number of alternative fitting procedures. 

# Coding style

* underscore (`_variable`) - a class member variable

# Notes

* **Documentation** in doxygen can be found [here](https://mrkonrad.github.io/OxShmolli2/html);
* **Continuous integration** was tested [in this repo](https://github.com/MRKonrad/ContinousIntegrationPlayground).
* **Gitmodules** - after `git clone` run `git submodule update --init --recursive` to get the submodules code

[travis_badge]: https://img.shields.io/travis/MRKonrad/OxShmolli2.svg?style=for-the-badge
[travis_link]: https://travis-ci.org/MRKonrad/OxShmolli2
[appveyor_badge]: https://img.shields.io/appveyor/ci/MRKonrad/OxShmolli2.svg?style=for-the-badge
[appveyor_link]: https://ci.appveyor.com/project/MRKonrad/oxshmolli2/
[coveralls_badge]: https://img.shields.io/coveralls/github/MRKonrad/OxShmolli2.svg?style=for-the-badge
[coveralls_link]: https://coveralls.io/github/MRKonrad/OxShmolli2
