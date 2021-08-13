#! /usr/bin/env Rscript

context("Permutation")
library(RedRibbon)

EPS  <- 0.00001

a <- as.double(1:1000)
b <- as.double(1:1000)

coord <- rrho_rectangle_min(1, 1, 1000, 1000, 100, 100, a, b, "hyper", "enrichment")
res <- rrho_ij(coord[1], coord[2], a, b, "hyper")
#print(res)
perm_res <- rrho_permutation(1, 1, 1000, 1000, a, b, algo_params=list(m=100, n=100), pvalue_i=coord[1], pvalue_j=coord[2])
#print(perm_res)

test_that(paste("pval < padj"), {
  expect(res$pvalue < perm_res$pvalue, paste("pval =", res$pvalue,  ">=", perm_res$pvalue))
})


## test permutation correlated LD FIT
N  <- 1000
N4  <- N %/% 4
half <- 10
r  <- half / (half + 1)
a <- as.double(1:N) / N
b <- as.double(1:N) / N
b[ (N4 + 1):N ]  <- runif(N - N4)
deps <- ifelse( (1:N %% 2) == 1, -1, 0:(N-1)  )
b  <- ifelse( (1:N %% 2) == 1, b, c(-1, r * b[1:(N-1)] - (1-r) * runif(N-1)) )

corr <- newLDFIT(position=1:N, deps=deps, half=10)

coord <- rrho_rectangle_min(1, 1, N, N, 100, 100, a, b, "hyper", "enrichment")
res <- rrho_ij(coord[1], coord[2], a, b, "hyper")
## print(res)

perm_res <- rrho_permutation(1, 1, N, N, a, b, algo_params=list(m=100, n=100), pvalue_i=coord[1], pvalue_j=coord[2])
## print(perm_res)

corr <- newLDFIT(position=0:999, deps=deps, half=10)
perm_res_corr <- rrho_permutation(1, 1, N, N, a, b, algo_params=list(m=100, n=100), correlation=corr, pvalue_i=coord[1], pvalue_j=coord[2])
## print(perm_res_corr)


print(paste("pval =", res$pvalue,  "; padj =", perm_res$pvalue, "; padj (LD) =", perm_res_corr$pvalue))
test_that(paste("pval < padj (permutation)"), {
  expect(res$pvalue < perm_res$pvalue, paste("pval =", res$pvalue,  ">=", perm_res$pvalue))
})

test_that(paste("padj (permutation) < padj (LD)"), {
  expect(perm_res$pvalue < perm_res_corr$pvalue, paste("pval =", perm_res$pvalue,  ">=", perm_res_corr$pvalue))
})


#> Test passed
