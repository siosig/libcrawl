```
$ mkdir {obj,dist}
$ make all
$ make install
```

# example: Makefile
```
CXXFLAGS=-Ofast -march=native -std=c++0x `pkg-config --cflags libcrawl` \
        `pkg-config --cflags glibmm-2.4` \
        `pkg-config --cflags libxml++-2.6` \
        -I/usr/local/mysql/include

CXXLIBFLAGS=`pkg-config --libs libcrawl` \
        `pkg-config --libs glibmm-2.4` \
        `pkg-config --libs libxml++-2.6` \
        -L/usr/local/mysql/lib -Wl,-rpath /usr/local/mysql/lib
```
