#!/usr/bin/env bash

git push --delete origin v0.4.2
git tag --delete v0.4.2
git tag v0.4.2 -m "2 parameter fitting"
git push origin v0.4.2