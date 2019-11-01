#!/usr/bin/env bash

git push --delete origin v0.4.3
git tag --delete v0.4.3
git tag v0.4.3 -m "new build"
git push origin v0.4.3