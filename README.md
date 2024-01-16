# Description

RedRibbon: a rank-rank hypergeometric overlap library.

# Installation

This procedure have been tested on debian/ubuntu but should work on any linux distribution.

## Directly from Github

In R, just run

```R
devtools::install_github("antpiron/RedRibbon")
```


## Alternative installation method from the tar.gz released package

In R, download RedRibbon_1.0-1.tar.gz on github, and then the run

```bash
R CMD INSTALL RedRibbon_1.0-1.tar.gz
```


## For advanced users only: C librairies if you plan to program in C (require root)

If you plan to program in C, standalone librairies can be installed.

We recommend the installation of GCC OpenMP support, the `GOMP` library. For Debian/Ubuntu:

```R
sudo apt-get install libgomp1
```

If not installed, while the libraries will be totally functionnal, no parallel execution will be available.


Download latest official release of the [`ale`](https://github.com/antpiron/ale) and
[`cRedRibbon`](https://github.com/antpiron/cRedRibbon) C libraries.


Install `ale` C library

```bash
tar xvzf ale-1.1.tar.gz
cd ale-1.1
./configure
make
sudo make install
```

Install `cRedRibbon` C library

```bash
tar xvzf  credribbon-1.1.tar.gz
cd credribbon-1.1
./configure
make
sudo make install
```

If the two C librairies are installed, the R package installation process will automatically detect those and dynamically link to those.

Rermark: if the libraries are not automaticaly detected, `sudo ldconfig /usr/local/lib/` might be necessary to run in order to update links and cache to the shared libraries.

# Documentation

A [R vignette with fully reproducible examples is available here](https://antpiron.github.io/RedRibbon.html). You can also open it from R with

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
ggRedRibbon(rr, quadrants=quad) + coord_fixed(ratio = 1, clip = "off")

## Get the down-down quandrant list of genes
df[quad$downdown$positions,]
```


# Citation

Please cite our [Life Science Alliance publication](https://doi.org/10.26508/lsa.202302203),

```text
Anthony Piron, Florian Szymczak, Theodora Papadopoulou, Maria Inês Alvelos, Matthieu Defrance, Tom Lenaerts, Décio L Eizirik, Miriam Cnop.
RedRibbon: A new rank-rank hypergeometric overlap for gene and transcript expression signatures.
Life Science Alliance. 2023 Dec 8;7(2):e202302203. doi: 10.26508/lsa.202302203. PMID: 38081640; PMCID: PMC10709657.
```
