# Description
TODO

# Install
## How to build
First, install both cRedRibbon and ale.

```
git clone <remote-repo> # TODO
cd <remote-repo> # TODO
autoconf
autoheader
```

## Check the package (requires devtools/roxygen2)
You may check the package:
```
R -e 'devtools::check()'
```

## Build the package from source
```
R -e 'devtools::build()'
```

Then install the package:
```
R CMD INSTALL RedRibbon_x.y-z.tar.gz
```