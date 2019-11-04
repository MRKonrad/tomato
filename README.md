# TOMATO
TOMATO (Total Mapping Toolbox) is a C++ library for the calculation of parametric maps in cardiac magnetic resonance 
imaging (MRI). As an open source project, TOMATO allows transparent and standardized cardiac longitudinal relaxation 
time (T1) mapping in clinical applications. With C++ implementation, TOMATO can easily interface and translate between 
research software environments, and commercial vendors’ closed-source C++ environments on scanners as well as 
post-processing software. To complement the core library implementation, a ready-to-use command line tool has been provided.

It contains ShMOLLI implementation as in [this article](https://jcmr-online.biomedcentral.com/articles/10.1186/1532-429X-12-69).   

| System                                         |     Status                                            |
|------------------------------------------------|:-----------------------------------------------------:|
| :tomato: [Tutorial][toamto_docs_link] :tomato: | :tomato: [Tomato Docs][toamto_docs_link]   :tomato:   |
| [Code documentation][doxygen_link]             | [Doxygen][doxygen_link]                               |
| [DOI][zenodo_link]                             | [![DOI][zenodo_badge]][zenodo_link]                   |
| [OSX/Linux build - Travis][travis_link]        | [![Travis (.org)][travis_badge]][travis_link]         |
| [Windows build - AppVeyor][appveyor_link]      | [![AppVeyor][appveyor_badge]][appveyor_link]          |
| [Code quality - Codacy][codacy_link]           | [![Codacy Badge][codacy_badge]][codacy_link]          |
| [Test coverage - Codecov][codecov_link]        | [![codecov][codecov_badge]][codecov_link]             |
| [Downloads][downloads_link]                    | [![GitHub Releases][downloads_badge]][downloads_link] |

## TomatoOpenSource and TomatoFull
**Important**

There are two `Tomato` version available: `TomatoOpenSource` compiled with publicly available code and `TomatoFull` 
containing additionally private code used for `AmoebaPrivateNr2` fitting algorithm using Nelder–Mead algorithm based 
on Numerical Recipes. Original ShMOLLI (and based on it Tomato) uses code based on Numerical Recipes book. Due to 
Numerical Recipes licence I cannot share this part of the code online. Please contact me if you would like to use this 
part of the code. To make up for this limitation we provide a number of alternative fitting procedures.

## Contributing and code of conduct

Please see [contributing.md](contributing.md) and [codeofconduct.md](codeofconduct.md).

## Troubleshooting

*   Problem: missing msvcp140.dll  

*   Solution: Download <https://www.microsoft.com/en-us/download/details.aspx?id=48145> as suggested in 
<https://stackoverflow.com/questions/32998902/msvcp140-dll-missing>

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
[doxygen_link]: https://mrkonrad.github.io/tomato/html/md__r_e_a_d_m_e.html
[toamto_docs_link]: https://mrkonrad.github.io/tomato_docs/
