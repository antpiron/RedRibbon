# Description

RedRibbon: a rank-rank hypergeometric overlap library.

# Installation

This procedure have been tested on debian/ubuntu but should work on any linux distribution.

## Dependencies

Download official release of the [`ale`]( https://github.com/antpiron/ale) and
[`cRedRibbon`](https://github.com/antpiron/ale) C libraries.


Install `ale` C library

```
tar xvzf ale-1.0.tar.gz
cd ale-1.0
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

Rermark: if the librairies are not automaticaly detected, `sudo ldconfig /usr/local/lib/` might be necessary to run in order to update links and cache to the shared libraries.

## R package

```
sudo R CMD INSTALL RedRibbon_0.1-1.tar.gz
```
