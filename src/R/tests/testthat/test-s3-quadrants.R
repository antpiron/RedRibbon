#! /usr/bin/env Rscript

context("Quadrants")
library(RedRibbon)

EPS  <- 0.000001

a <- as.double(1:1000) - 450
b <- as.double(1:1000) - 460

rr <- newRRHO(a, b)

quad <- quadrants(rr, m=1000, n=1000)

test_that(paste("Testing whole quadrants"), {
    expect(is.list(quad), paste("type(quad) = ", typeof(quad)))
    expect(is.list(quad$whole),  paste("type(quad$whole) = ", typeof(quad$whole)))
    expect(quad$whole$i == 500,  paste("quad$whole$i = ", quad$whole$i, "!= 500"))
    expect(quad$whole$j == 500,  paste("quad$whole$j = ", quad$whole$j, "!= 500"))
    expect(length(quad$whole$positions) == 500,  paste("length(quad$whole$positions) = ", length(quad$whole$positions), "!= 500"))
})

quad <- quadrants(rr, whole=FALSE, m=1000, n=1000)
test_that(paste("Testing quadrants"), {
    expect(is.list(quad), paste("type(quad) = ", typeof(quad)))
    expect(is.list(quad$downdown),  paste("type(quad$downdown) = ", typeof(quad$downdown)))
    expect(is.list(quad$upup),  paste("type(quad$upup) = ", typeof(quad$upup)))
    
    expect(quad$downdown$i == 449,  paste("quad$downdown$i = ", quad$downdown$i, "!= 449"))
    expect(quad$downdown$j == 449,  paste("quad$downdown$j = ", quad$downdown$j, "!= 449"))
    expect(length(quad$downdown$positions) == 449,
           paste("length(quaddowndown$positions) = ", length(quad$downdown$positions), "!= 449"))
    
    expect(quad$upup$i == 500,  paste("quad$upup$i = ", quad$upup$i, "!= 500"))
    expect(quad$upup$j == 500,  paste("quad$upup$j = ", quad$upup$j, "!= 500"))
    expect(length(quad$upup$positions) == 501,
           paste("length(quadupup$positions) = ", length(quad$upup$positions), "!= 501"))
})

a <- as.double(1:1000) - 500 - 1
b <-rev(a)

rr <- newRRHO(a, b)

quad <- quadrants(rr, whole=FALSE, m=1000, n=1000)
test_that(paste("Testing quadrants"), {
    expect(is.list(quad), paste("type(quad) = ", typeof(quad)))
    expect(is.list(quad$updown),  paste("type(quad$updown) = ", typeof(quad$updown)))
    expect(is.list(quad$downup),  paste("type(quad$downup) = ", typeof(quad$downup)))
    
    expect(quad$downup$i == 499,  paste("quad$downup$i = ", quad$downup$i, "!= 499"))
    expect(quad$downup$j == 501,  paste("quad$downup$j = ", quad$downup$j, "!= 501"))
    expect(length(quad$downup$positions) == 499,
           paste("length(quaddownup$positions) = ", length(quad$downup$positions), "!= 499"))
    
    expect(quad$updown$i == 501,  paste("quad$updown$i = ", quad$updown$i, "!= 501"))
    expect(quad$updown$j == 499,  paste("quad$updown$j = ", quad$updown$j, "!= 499"))
    expect(length(quad$updown$positions) == 499,
           paste("length(quadupdown$positions) = ", length(quad$updown$positions), "!= 499"))
})



