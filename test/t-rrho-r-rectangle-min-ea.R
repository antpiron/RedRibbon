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

coord <- .Call("rrho_r_rectangle_min_ea", as.integer(1), as.integer(1), as.integer(1000), as.integer(1000),
             as.double(a), as.double(b), as.character("hyper"), as.character="enrichment")

if ( 2 != length(coord) )
    stop(paste("Length(coord) =", length(coord),  "!=", 2))
