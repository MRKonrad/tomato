# Contributing to TOMATO
We want to make contributing to this project as easy and transparent as
possible.

## Our Development Process
We use GitHub to sync code to and from our internal repository. We'll use GitHub
to track issues and feature requests, as well as accept pull requests.

## Issues
We use GitHub issues to track public bugs. Please ensure your description is
clear and has sufficient instructions to be able to reproduce the issue.

## Report bugs using Github's [issues](https://github.com/MRKonrad/tomato/issues)
We use GitHub issues to track public bugs. Report a bug by [opening a new issue](); it's that easy!


## Pull Requests
We actively welcome your pull requests.

1. Fork the repo and create your branch from `master`.
2. If you've added code that should be tested, add tests.
3. If you've changed APIs, update the documentation.
4. Ensure the test suite passes.
5. Make sure your code lints.
6. Issue that pull request!

## Any contributions you make will be under the MIT Software License
In short, when you submit code changes, your submissions are understood to be under the same 
[MIT License](http://choosealicense.com/licenses/mit/) that covers the project. 
Feel free to contact the maintainers if that's a concern.

## Coding style
* Write code compatible with C++98
* Use underscore (`_variable`) in the name of a class member variable.
* C++98 does not allow me to use constructor delegation. Using methods like `setAllPointersToNull()` to make up for it.
* Order of methods in a class definition:
    * functional (calculation, helpers etc)
    * getters/setters
    * disp()
    * constructors/destructors
* In **copy constructors** copy primitive types, set all pointers to zero.

## License
By contributing to Draft.js, you agree that your contributions will be licensed
under its BSD license.

## References
This document was adapted from the open-source contribution guidelines for 
[Facebook's Draft](https://github.com/facebook/draft-js/blob/a9316a723f9e918afde44dea68b5f9f39b7d9b00/CONTRIBUTING.md) 
and [qMRLab](https://github.com/qMRLab/qMRLab/blob/master/CONTRIBUTING.md).