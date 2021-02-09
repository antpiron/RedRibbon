#! /usr/bin/env Rscript

context("Quadrants")
library(RedRibbon)

EPS  <- 0.000001

a <- as.double(1:1000) - 450
b <- as.double(1:1000) - 460

rr <- newRRHO(a, b)

quad <- quadrants(rr, m=1000, n=1000)

print(quad)
test_that(paste("Testing whole quadrants"), {
    expect(is.list(quad), paste("type(quad) = ", typeof(quad)))
    expect(is.list(quad$whole),  paste("type(quad$whole) = ", typeof(quad$whole)))
    expect(quad$whole$i == 500,  paste("quad$whole$i = ", quad$whole$i, "!= 500"))
    expect(quad$whole$j == 500,  paste("quad$whole$j = ", quad$whole$j, "!= 500"))
    expect(length(quad$whole$positions) == 500,  paste("length(quad$whole$positions) = ", length(quad$whole$positions), "!= 500"))
})

quad <- quadrants(rr, whole=FALSE, m=1000, n=1000)

print(quad)


#> Test passed
