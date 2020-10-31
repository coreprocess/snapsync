# snapsync

An excellent library and toolset for optimized transfers of single files and directory snapshots based on librsync.

![npm downloads total](https://img.shields.io/npm/dt/snapsync.svg) ![npm version](https://img.shields.io/npm/v/snapsync.svg) ![npm license](https://img.shields.io/npm/l/snapsync.svg)

librsync helps with the efficient calculation of the differences between two files. The rsync algorithm is different from most differencing algorithms because it does not require the presence of the two files to calculate the delta. Instead, it requires a set of checksums of each block of one file, which together form a signature for that file. Blocks at any in the other file which have the same checksum are likely to be identical, and whatever remains is the difference.

## Installation

Install the `snapsync` module via

```sh
npm install snapsync --save
```

or

```sh
yarn add snapsync
```

This npm-library provides prebuilt binaries for a particular set of platforms with the help of [node-pre-gyp](https://www.npmjs.com/package/node-pre-gyp) and [node-pre-gyp-github](https://www.npmjs.com/package/node-pre-gyp-github). In case there are no prebuilt binaries for your platform, the installation procedure tries to fall back to building the library from source code. Building from source code requires further dependencies which cannot be provided via NPM. Therefore building from source code will fail in this context. If you want to build the module for your architecture, please follow [these instructions](https://github.com/coreprocess/snapsync). Or [create a ticket](https://github.com/coreprocess/snapsync/issues/new) stating your requirements and I will include your architecture in our regular builds ASAP.

## Usage

### Synchronize Files

*Scenario:* You have two devices (A and B) each storing a different version of a particular file, and you want to update the version on device B to the version provided in device A.

```js
var snapsync = require('snapsync');

// API:
// 1) snapsync.sync.signature(<base:in>, <signature:out> [, block-length] [, sum-length])
// 2) snapsync.sync.delta(<signature:in>, <target:in>, <patch:out>)
// 3) snapsync.sync.patch(<patch:in>, <base:in>, <target:out>)

// on device B: create signature file (and copy it to device A)
snapsync.sync.signature('bin-v1', 'bin-v1.sig', 0, 8);

// on device A: create patch file (and copy it to device B)
snapsync.sync.delta('bin-v1.sig', 'bin-v2', 'bin-v1v2.patch');

// on device B: apply patch file to old version to generate new version
snapsync.sync.patch('bin-v1v2.patch', 'bin-v1', 'bin-v2');
```

*Note:* You can use this procedure with any filetype. In case you are planning to use archive files like tar or zip to synchronize directory structures, please use our optimized file format instead (see below).

### Packing and Unpacking Directory Structures

The archive format provided by the `snapsync.snap` tool is optimized for the signature, delta and patch cycle provided by `snapsync.sync`. It works with recursive directory structures and stores filenames, access bitmasks and content of files. Additionally it sorts directories while creating the image file to avoid unnecessary deltas during the synchronization phase.

```js
var snapsync = require('snapsync');

// API:
// 1) snapsync.snap.create(<directory:in>, <image:out>)
// 2) snapsync.snap.extract(<image:in>, <directory:out>)

// create image
snapsync.snap.create('my-data', 'my-data.img');

// extract image
snapsync.snap.extract('my-data.img', 'my-data');
```
