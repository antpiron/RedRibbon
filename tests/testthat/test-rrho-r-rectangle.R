#! /usr/bin/env Rscript

context("Rectangle")
library(RedRibbon)

EPS  <- 0.00001

a <- as.double(1:1000)
b <- as.double(1:1000)

mat  <- rrho_rectangle(1, 1, 1000, 1000, 10, 10, a, b, "hyper", FALSE )


expected <- 1.0 / 1000
diff  <- abs(expected - mat[1,1])

test_that(paste("mat[1,1] ==", expected), {
  expect(diff <=  EPS * 1.0, paste("mat[1,1] ==", mat[1,1], "!=", expected))
})
#> Test passed

EPS  <- 0.001
mat  <- rrho_rectangle(1, 1, 1000, 1000, 10, 10, a, b, "hyper", TRUE )

expected <- -log(1.0 / 1000)
diff  <- abs(expected - mat[1,1])

test_that(paste("-log mat[1,1] ==", expected), {
  expect(diff <=  EPS * 1.0, paste("mat[1,1] ==", mat[1,1], "!=", expected))
})
#> Test passed



