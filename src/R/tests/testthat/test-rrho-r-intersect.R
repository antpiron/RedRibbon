#! /usr/bin/env Rscript

context("Rectangle minimum")
library(RedRibbon)

EPS  <- 0.00001

a <- as.double(1:1000)
b <- as.double(1:1000)

downdown <- rrho_intersect(10, 10, a, b, "downdown")


test_that("downdown", {
    expect(10 == length(downdown), paste("Length(downdown) =", length(downdown),  "!=", 10))
    expect(all(downdown[1:10] == 1:10), paste("downdown[1:10]", paste(downdown[1:10], collapse= " ")))
})



#> Test passed
