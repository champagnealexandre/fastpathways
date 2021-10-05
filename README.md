# FastPathways

## Building from Source
To build *FastPathways*, you will need to have https://cmake.org/[CMake] v2.8 or higher.

### Linux, OS X, and Windows (Bash, MinGW and Cygwin)
Once CMake is installed, building, testing and installing the library is a snap
```shell
$ cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Release
$ make -C build
```

This will compile down to shared and static libraries, and generate two executables (at the moment):
`build/intchains` and `build/test/fastpathways_unittests`.

You can run the unit tests explicitly
```shell
$ ./build/test/fastpathways_unittests
```
or via make
```shell
$ make -C build test
```

## libfastpathways

The library, for the most part, exposes a single function `thurber(int64_t n) -> int64_t`. The
argument is the integer for which you wish to compute the assembly index (minimal length of all
addition chains terminating at `n`), and obviously the return value is that assembly index.

### Usage
```cpp
#include <fastpathways.h>
#include <iostream>

auto main(int argc, char **argv) -> int {
    std::cout << thurber(607) << std::endl;
}
```

Output:
```
13
```

## Integer Addition Chains

```shell
$ ./build/intchains
usage: intchain [subcommand] n

Subcommands:
    <none>: compute and output the assembly index of `n`
    time:   compute and output the assembly index of `n` and run time.
    perf:   compute the assembly index of all integers from 1 to `n` (inclusive)
            and output the sum and run time.
Exception: ./build/intchains exited with 255
[tty 6], line 1: ./build/intchains
$ ./build/intchains 607
13
$ ./build/intchains time 607
13
0.04888121s
$ ./build/intchains perf 1000
10808
2.83965543s
```
