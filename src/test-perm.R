#! /usr/bin/env Rscript


library(magrittr)
library(RedRibbon)
library(rucdr)

n <- 10000
nh <- n / 2

############### Perfect

## New
rr <- newRRHO(1:n, 1:n) 
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
rr <- newRRHO(1:n, n:1) 
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
rr <-  newRRHO(a, b) %>% setoptions(enrichment_mode = "hyper-two-tailed")
quadrants <- quadrants(rr, whole=FALSE, m=100, n=100, permutation=TRUE)
gg <- ggplot(rr, quadrants=quadrants)
ggsave("/tmp/new-rrho-perfect-quad-10000.png", gg)

## Old
oldrr <- RRHO(a, b, stepsize=n/500)
gg <- plot(oldrr, labels=c("a", "b"), colors = c(mycol, rev(mycol)))
ggsave("/tmp/old-rrho-perfect-quad-10000.png", gg)

############ Perfect Quad

## New
nq <- n / 2
aq <- (1:nq) - nq / 2 - 1
raq <-  rev(aq)
a <- c(aq, aq)
b <- c(aq, raq)
names(a)  <- 1:n
names(b)  <- 1:n
rr <-  newRRHO(a, b) ## %>% setoptions(enrichment_mode = "hyper-two-tailed")
quadrants <- quadrants(rr, whole=FALSE)
gg <- ggplot(rr, quadrants=quadrants)
ggsave("/tmp/new-rrho-perfect-quad-10000.png", gg)


############ Imperfect
n.perf <-  n * 10 / n
a <- c(1:n.perf, sample(n-n.perf))
b <- c(1:n.perf, sample(n-n.perf))
rr <-  newRRHO(a, b)
quadrants <- quadrants(rr, whole=TRUE)
gg <- ggplot(rr, quadrants=quadrants)

gg

n <- 1000
a <- (1:n) - 500
b <- (1:n) - 500
rr <- newRRHO(a, b)
quadrants <- quadrants(rr, whole=FALSE)
ggplot(rr, quadrants=quadrants)
