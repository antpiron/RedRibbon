#! /usr/bin/env Rscript

context("Expression-prediction")
library(RedRibbon)

eps <- 0.00001

N.samples  <- 5
N.genes <- 10

mat <- matrix((rnorm(N.samples * N.genes, sd = 10) - 0.5), ncol=N.samples)
mat[2,]  <- mat[1,]

print(mat)

res <- rrho_expression_prediction(mat)
print(res)


test_that(paste("index1"), {
   expect(res$index[1] * res$index[2] < 0, paste("index[1] =", res$index[1],  "; index[2]", res$index[2]))
   expect( (2 == res$index[1] && -1 == res$index[2]) || (-1 == res$index[1] && 1 == res$index[2]),
          paste("index[1] =", res$index[1],  "; index[2]", res$index[2]))
})

print(head(res$beta))

test_that(paste("beta1"), {
    expect( ( abs(1 - res$beta[1,2]) < eps && abs(res$beta[2,2]) < eps) ||
            ( abs(res$beta[1,2]) < eps && abs(1 - res$beta[2,2]) < eps),
           paste("abs[1,2] =", res$beta[1,2],  "; beta[2,2]", res$beta[2,2]))
})

## test with 0

mat[N.genes,]  <- 0
mat[N.genes-1,1]  <- 0
## print(mat)

res <- rrho_expression_prediction(mat)
## print(res)


test_that(paste("index2"), {
   expect(res$index[1] * res$index[2] < 0, paste("index[1] =", res$index[1],  "; index[2]", res$index[2]))
   expect( (2 == res$index[1] && -1 == res$index[2]) || (-1 == res$index[1] && 1 == res$index[2]),
          paste("index[1] =", res$index[1],  "; index[2]", res$index[2]))
})

## print(head(res$beta))

test_that(paste("beta2"), {
    expect( ( abs(1 - res$beta[1,2]) < eps && abs(res$beta[2,2]) < eps) ||
            ( abs(res$beta[1,2]) < eps && abs(1 - res$beta[2,2]) < eps),
           paste("abs[1,2] =", res$beta[1,2],  "; beta[2,2]", res$beta[2,2]))
})
