#! /usr/bin/env Rscript

context("Permutation")
library(RedRibbon)

EPS  <- 0.00001

a <- as.double(1:1000)
b <- as.double(1:1000)

coord <- rrho_rectangle_min(1, 1, 1000, 1000, 100, 100, a, b, "hyper", "enrichment")
res <- rrho_ij(coord[1], coord[2], a, b, "hyper")
perm_res <- rrho_permutation(1, 1, 1000, 1000, a, b, algo_param=list(m=100, n=100), pvalue=res$pvalue)


test_that(paste("pval < padj"), {
  expect(res$pvalue < perm_res$pvalue, paste("pval =", res$pvalue,  ">=", perm_res$pvalue))
})



#> Test passed
