#! /usr/bin/env Rscript

context("Normalize")
library(RedRibbon)

eps <- 0.000001

m <- 12
n <- 10

ref <- runif(m ,min=1, max=10000)
beta <- c(1, runif(n-1, min=1, max=10))
mat <- do.call(cbind, lapply(1:n, function (x) beta[x] * ref ) )

norm.mat <- rrho_normalize(mat, 1:3)

## print(beta)
## print(mat)
## print(norm.mat)

test_that(paste("Diff"), {
    res <- sum( (norm.mat - rowMeans(mat))^2 )
    expect( res  <= eps, paste("Does not seem normalized. res =", res, ">", eps))
})
