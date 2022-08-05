# Description

RedRibbon: a rank-rank hypergeometric overlap library.

# Installation

This procedure have been tested on debian/ubuntu but should work on any linux distribution.

```
sudo R CMD INSTALL RedRibbon_0.2-1.tar.gz
```


## For advanced user

The two C librairies can independently be installed before installing the R package. The R package will automatically detect those dependencies if present.

We recommend the installation of GCC OpenMP support, the `GOMP` library. For Debian/Ubuntu:

```
sudo apt-get install libgomp1
```

If not installed, while the libraries will be totally functionnal, no parallel execution will be available.


Download official release of the [`ale`]( https://github.com/antpiron/ale) and
[`cRedRibbon`](https://github.com/antpiron/cRedRibbon) C libraries.


Install `ale` C library

```
tar xvzf ale-1.1.tar.gz
cd ale-1.1
./configure
make
sudo make install
```

Install `cRedRibbon` C library

```
tar xvzf credribbon-1.0.tar.gz
cd credribbon-1.0
./configure
make
sudo make install
```

Rermark: if the libraries are not automaticaly detected, `sudo ldconfig /usr/local/lib/` might be necessary to run in order to update links and cache to the shared libraries.

