# Chillco Islandora Large Image Solution Pack

## Introduction

A test implementation of large images which consists of the following datastreams:
MODS
OBJ (mimetype image/tiff)
IMAGE (mimetype image/jpg,image/png,image/gif)
LARGE (mimetype image/jpg,image/png,image/gif)
MEDIUM (mimetype image/jpg,image/png,image/gif)
TN (mimetype image/jpg,image/png,image/gif)

This can will ingest and create derivatives off the OBJ or IMAGE datastreams (preference given to OBJ if both are provided). It is done via image style presets which are alterable by the front-end site admin (no hard presets). This also allowed to get familiar with the Islandora APIs. The large image is a lossy conversion (via imagemagick) of the OBJ datastream if it exists. Works well with jQuery Zoom module.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [ImageMagick](https://drupal.org/project/imagemagick)

## Installation

For the module, install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

You need to configure the path to imagemagick (or the 'convert' command)

## Troubleshooting

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## FAQ

Why not use the Large Image Solution Pack instead?

This was primarily to get familiar with the Islandora Content Models and APIs. It does a few things in better ways than the official image solution pack (large image of tiff is web-friendly and user-configurable image presets, for example) and the official image solution pack does a few things better than this (works with OpenSeaDragon, larger pool of contributors, to name a few). Some of the changes from this solution pack (namely the user-configuration image presets) will be patched and pushed for a pull request into the image solution packs.

## Maintainers

[Ashok Modi](https://github.com/btmash)
