#! /usr/bin/env Rscript

EPS  <- 0.00001

LIBPATH  <- "src/.libs/librrho.so"
if ( ! file.exists(LIBPATH) )
    LIBPATH  <-  "../src/.libs/librrho.so"
if ( ! file.exists(LIBPATH) )
    stop("Unable to find librrho.so")


dyn.load(LIBPATH)

a <- as.double(1:1000)
b <- as.double(1:1000)

mat  <- .Call("rrho_r_rectangle", as.integer(1), as.integer(1), as.integer(1000), as.integer(1000),
              as.integer(10), as.integer(10), as.double(a), as.double(b), as.character("hyper"))


expected <- 1.0 / 1000
diff  <- abs(expected - mat[1,1])
if ( diff > EPS * 1.0)
    stop(paste(mat[1,1], "!=", expected))
