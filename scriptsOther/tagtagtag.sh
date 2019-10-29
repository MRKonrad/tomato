#!/usr/bin/env bash

git push --delete origin v0.4.1
git tag --delete v0.4.1
git tag v0.4.1 -m "updating documentation"
git push origin v0.4.1