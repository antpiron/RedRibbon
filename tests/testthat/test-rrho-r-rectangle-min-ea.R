#! /usr/bin/env Rscript

context("Rectangle minimum EA")
library(RedRibbon)

EPS  <- 0.00001

a <- as.double(1:1000)
b <- as.double(1:1000)

coord <- rrho_rectangle_min_ea(1, 1, 1000, 1000, a, b, "hyper", "enrichment")


test_that(paste("Length(coord) == 2"), {
  expect(2 == length(coord), paste("Length(coord) =", length(coord),  "!=", 2))
})

test_that(paste("coord == (500,500)"), {
  expect(all( coord == c(500, 500) ) , paste("coord = (", coord[1], ", ", coord[2], ") != (500, 500)"))
})

#> Test passed
