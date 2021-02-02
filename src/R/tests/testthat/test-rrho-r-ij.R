#! /usr/bin/env Rscript

context("RHHO ij")
library(RedRibbon)

EPS  <- 0.000001

a <- as.double(1:1000)
b <- as.double(1:1000)

result <- rrho_ij(1, 1, a, b, "hyper")


test_that(paste("is.list(result)?"), {
  expect(is.list(result), paste("type(result) = ", result))
})

test_that(paste("length(result) == 4"), {
  expect(length(result) == 4, paste("length(result) = ", length(result)))
})

test_that(paste("result$pvalue == 1/1000"), {
  expect(abs(result$pvalue - 1/1000) < EPS, paste("result$pvalue = ", result$pvalue))
})

test_that(paste("result$direction > 0"), {
  expect(result$direction > 0, paste("result$direction = ", result$direction))
})

test_that(paste("result$count == 1"), {
  expect(result$count == 1, paste("result$count = ", result$count))
})

result <- rrho_ij(500, 500, a, b, "hyper")

test_that(paste("result$count == 500"), {
  expect(result$count == 500, paste("result$count = ", result$count))
})



#> Test passed
