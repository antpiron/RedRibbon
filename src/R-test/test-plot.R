#! /usr/bin/env Rscript


library(magrittr)
library(RedRibbon)
library(rucdr)

n <- 10000
nh <- n / 2

############### Perfect

## New
rr <- RedRibbon(1:n, 1:n) 
gg <- ggplot(rr)
ggsave("/tmp/new-rrho-perfect-10000.png", gg)
rectangle_min(rr,1,1,n,n)

## Old
colfunc <- colorRampPalette(c("#eb3434", "#eb9334", "#ebeb34", "#49eb34", "#34eba5", "#34b4eb", "#3446eb"))
mycol = colfunc(1000)


a <- c(1:n) - nh
names(a)  <- 1:n 
b <- a
oldrr <- RRHO(a, b, stepsize=n/500)
gg <- plot(oldrr, labels=c("a", "b"), colors = c(mycol, rev(mycol)))
ggsave("/tmp/old-rrho-perfect-10000.png", gg)


############### Perfect reversed
rr <- RedRibbon(1:n, n:1) 
ggplot(rr)
rectangle_min(rr,1,1,n,n, direction="")
rrho_ij(nh,nh, rr$data$a, rr$data$b)


############ Perfect Quad

## New
nq <- n / 4
aq <- (1:nq) - nq / 2
raq <-  rev(aq)
a <- c(aq, aq, raq, raq)
b <- c(aq, raq, aq, raq)
names(a)  <- 1:n
names(b)  <- 1:n
rr <-  RedRibbon(a, b) ## %>% setoptions(enrichment_mode = "hyper-two-tailed")
gg <- ggplot(rr)
ggsave("/tmp/new-rrho-perfect-quad-10000.png", gg)
rectangle_min(rr,1,1,n,n)
rectangle_min(rr,1,1,n,n, direction="")

## Old
oldrr <- RRHO(a, b, stepsize=n/500)
gg <- plot(oldrr, labels=c("a", "b"), colors = c(mycol, rev(mycol)))
ggsave("/tmp/old-rrho-perfect-quad-10000.png", gg)
