#! /usr/bin/env Rscript

context("Quadrants permutations")
library(RedRibbon)

EPS  <- 0.000001

a <- as.double(1:1000) - 450
b <- as.double(1:1000) - 460

rr <- newRRHO(a, b)

quad <- quadrants(rr, m=100, n=100, permutation=TRUE)
test_that(paste("Testing whole quadrants"), {
    expect(is.list(quad), paste("type(quad) = ", typeof(quad)))
    expect(is.list(quad$whole),  paste("type(quad$whole) = ", typeof(quad$whole)))
    expect(quad$whole$log_pvalue > quad$whole$log_padj, paste("log_pvalue =  ", quad$whole$pvalue, "<=", quad$whole$log_padj, "= log_padj"))
    expect(! is.infinite(quad$whole$log_padj), "log_padj is inf ")
})

quad <- quadrants(rr, whole=FALSE, m=100, n=100, permutation=TRUE)
test_that(paste("Testing quadrants"), {
    expect(is.list(quad), paste("type(quad) = ", typeof(quad)))
    expect(is.list(quad$downdown),  paste("type(quad$downdown) = ", typeof(quad$downdown)))
    expect(is.list(quad$upup),  paste("type(quad$upup) = ", typeof(quad$upup)))
    expect(quad$downdown$log_pvalue > quad$downdown$log_padj, paste("downdown pvalue =  ", quad$downdown$log_pvalue, "<=", quad$downdown$log_padj, "= log_padj"))
    expect(! is.infinite(quad$downdown$log_padj), "downdown log_padj is inf ")
    expect(quad$upup$log_pvalue > quad$upup$log_padj, paste("upup log_pvalue =  ", quad$upup$log_pvalue, "<=", quad$upup$log_padj, "= log_padj"))
    expect(! is.infinite(quad$upup$log_padj), "upup log_padj is inf ")
})

a <- as.double(1:2000) - 500 - 1
b <-rev(a)

rr <- newRRHO(a, b)
rr <- setoptions(rr, enrichment_mode="hyper-two-tailed")

quad <- quadrants(rr, whole=FALSE, m=100, n=100, permutation=TRUE)
test_that(paste("Testing quadrants (two-tailed)"), {
    expect(is.list(quad), paste("type(quad) = ", typeof(quad)))
    expect(is.list(quad$updown),  paste("type(quad$updown) = ", typeof(quad$updown)))
    expect(is.list(quad$downup),  paste("type(quad$downup) = ", typeof(quad$downup)))
    expect(quad$updown$log_pvalue > quad$updown$log_padj, paste("updown log_pvalue =  ", quad$updown$log_pvalue, "<=", quad$updown$log_padj, "= log_padj"))
    expect(! is.infinite(quad$updown$log_padj), "updown log_padj is inf ")
    expect(quad$downup$log_pvalue > quad$downup$log_padj, paste("downup log_pvalue =  ", quad$downup$log_pvalue, "<=", quad$downup$log_padj, "= log_padj"))
    expect(! is.infinite(quad$downup$log_padj), "downup log_padj is inf ")
})



