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

a <- as.double(1:1000) / 1000
b <- as.double(1:1000) / 1000
coord <- rrho_rectangle_min(1, 1, 1000, 1000, 100, 100, a, b, "hyper", "enrichment")
res <- rrho_ij(coord[1], coord[2], a, b, "hyper")
## print(res)

perm_res <- rrho_permutation(1, 1, 1000, 1000, a, b, algo_params=list(m=100, n=100), pvalue_i=coord[1], pvalue_j=coord[2])
## print(perm_res)

corr <- newLDFIT(position=0:999, half=10)
perm_res_corr <- rrho_permutation(1, 1, 1000, 1000, a, b, algo_params=list(m=100, n=100), correlation=corr, pvalue_i=coord[1], pvalue_j=coord[2])
## print(perm_res_corr)

test_that(paste("pval < padj (permutation)"), {
  expect(res$pvalue < perm_res$pvalue, paste("pval =", res$pvalue,  ">=", perm_res$pvalue))
})

test_that(paste("padj (permutation) < padj (LD)"), {
  expect(perm_res$pvalue < perm_res_corr$pvalue, paste("pval =", perm_res$pvalue,  ">=", perm_res_corr$pvalue))
})


#> Test passed
