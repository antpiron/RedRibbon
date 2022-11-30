# Description

RedRibbon: a rank-rank hypergeometric overlap library.

# Installation

This procedure have been tested on debian/ubuntu but should work on any linux distribution.

## Directly from Github

In R, run

```
devtools::install_github("antpiron/RedRibbon")
```


## Alternative installation method from the tar.gz released package

In R, download RedRibbon_0.2-1.tar.gz on github, and then the run

```
R CMD INSTALL RedRibbon_0.2-1.tar.gz
```


## For advanced user (For shared installation, require root)

The two C librairies can independently be installed before installing the R package. The R package will automatically detect those dependencies if present.

We recommend the installation of GCC OpenMP support, the `GOMP` library. For Debian/Ubuntu:

```
sudo apt-get install libgomp1
```

If not installed, while the libraries will be totally functionnal, no parallel execution will be available.


Download official release of the [`ale`](https://github.com/antpiron/ale) and
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
tar xvzf  credribbon-1.1.tar.gz
cd credribbon-1.1
./configure
make
sudo make install
```

Rermark: if the libraries are not automaticaly detected, `sudo ldconfig /usr/local/lib/` might be necessary to run in order to update links and cache to the shared libraries.

# Documentation

A R vignette is available. for the whole documentation, run in R

```R
vignette("RedRibbon")
```


Short summary,

```R
library(RedRibbon)

## df is a data.frame with 3 columns: id, a, b
## 
##    id 	   a 	   b
## gene1 	-499 	-499
## gene2 	-498 	-498
## gene3 	-497 	-497
## gene4 	-496 	-496
## gene5 	-495 	-495
## gene6 	-494 	-494 
## ...

## Create RedRibbon object
rr <- RedRibbon(df, enrichment_mode="hyper-two-tailed")

## Run the overlap using evolutionnary algorithm,
## computing permutation adjusted p-value for the four quadrants
quad <- quadrants(rr, algorithm="ea", permutation=TRUE, whole=FALSE)

## Plots the results
ggRedRibbon(rr, quadrants=quad) + coord_fixed(ratio = 1)

## Get the down-down quandrant list of genes
df[quad$downdown$positions,]
```


# Citation

Please cite the [biorxiv pre-print](https://doi.org/10.1101/2022.08.31.505818),

```
RedRibbon: A new rank-rank hypergeometric overlap pipeline to compare gene and transcript expression signatures
Anthony Piron, Florian Szymczak, Maria Inês Alvelos, Matthieu Defrance, Tom Lenaerts, Décio L. Eizirik, Miriam Cnop
bioRxiv 2022.08.31.505818; doi: https://doi.org/10.1101/2022.08.31.505818 
```
