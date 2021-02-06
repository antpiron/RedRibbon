#! /usr/bin/env Rscript

context("Intersect")
library(RedRibbon)

EPS  <- 0.00001

a <- as.double(1:1000)
b <- as.double(1:1000)

downdown <- rrho_intersect(10, 10, a, b, "downdown")


test_that("downdown", {
    expect(10 == length(downdown), paste("Length(downdown) =", length(downdown),  "!=", 10))
    expect(all(downdown[1:10] == 1:10), paste("downdown[1:10]", paste(downdown[1:10], collapse= " ")))
})

upup <- rrho_intersect(991, 991, a, b, "upup")

test_that("upup", {
    expect(10 == length(upup), paste("Length(upup) =", length(upup),  "!=", 10, "upup = ",  paste(upup[1:10], collapse= " ")))
    expect(all(upup[1:10] %in% 991:1000), paste("upup[1:10]", paste(upup[1:10], collapse= " ")))
})

downup <- rrho_intersect(10, 991, a, rev(b), "downup")

test_that("downup", {
    expect(10 == length(downup), paste("Length(downup) =", length(downup),  "!=", 10))
    expect(all(downup[1:10] == 1:10), paste("downup[1:10]", paste(downup[1:10], collapse= " ")))
})

updown <- rrho_intersect(991, 10, a, rev(b), "updown")

test_that("updown", {
    expect(10 == length(updown), paste("Length(updown) =", length(updown),  "!=", 10))
    expect(all(updown[1:10] == 991:1000), paste("updown[1:10]", paste(updown[1:10], collapse= " ")))
})


#> Test passed
