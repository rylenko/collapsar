# (WIP) Collapsar 🎇

A simple program for sending and receiving compressed files.

# Supported compression algorithms

Lossless:

- [Huffman coding](https://wikipedia.org/wiki/Huffman_coding).

# Installation

Building system is [CMake](https://en.wikipedia.org/wiki/CMake).

### Release dependencies

- GUI —  [Qt 6](https://www.qt.io/product/qt6).

### Debug dependencies

- GUI —  [Qt 6](https://www.qt.io/product/qt6).
- Testing library —  [Google Test](https://en.wikipedia.org/wiki/Google_Test).
- Undefined behavior sanitizer —  [ubsan](https://github.com/gcc-mirror/gcc/tree/master/libsanitizer/ubsan).
- Address sanitizer —  [asan](https://github.com/gcc-mirror/gcc/tree/master/libsanitizer/asan).

### Development build

1. Prepare build directory:

```
$ mkdir build
$ cd build
$ cmake ..
```

2. Build the project:

```
$ cmake --build .
```

3. Test it if you need:

```
$ ./test
```

4. Run a development build:

```
$ ./collapsar
```

### Release build

Coming soon...

# Ideas

- Support [tar](https://wikipedia.org/wiki/Tar_(computing))?
