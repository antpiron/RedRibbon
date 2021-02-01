

## SEXP rrho_r_rectangle(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP m, SEXP n, SEXP a, SEXP b, SEXP mode, SEXP log_flag)

rrho_rectangle  <- function (i, j, i.len, j.len, m, n, a, b, mode=c("hyper"), LOG=FALSE)
{
    .Call("rrho_r_rectangle", as.integer(i), as.integer(j), as.integer(i.len), as.integer(j.len),
          as.integer(m), as.integer(n), as.double(a), as.double(b), as.character(mode), as.logical(LOG))
}

rrho_rectangle_min  <- function (i, j, i.len, j.len, m, n, a, b, mode=c("hyper"), direction)
{
    .Call("rrho_r_rectangle_min", as.integer(i), as.integer(j), as.integer(i.len), as.integer(j.len),
          as.integer(m), as.integer(n), as.double(a), as.double(b), as.character(mode), as.character(direction))
}


rrho_rectangle_min_ea  <- function (i, j, i.len, j.len, a, b, mode=c("hyper"), direction="enrichment")
{
    .Call("rrho_r_rectangle_min_ea", as.integer(i), as.integer(j), as.integer(i.len), as.integer(j.len),
             as.double(a), as.double(b), as.character(mode), as.character(direction))
}

## SEXP rrho_r_rrho(SEXP i, SEXP j, SEXP a, SEXP b, SEXP mode)
rrho_ij  <- function (i, j, a, b, mode=c("hyper"))
{
    .Call("rrho_r_rrho", as.integer(i), as.integer(j), as.double(a), as.double(b), as.character(mode))
}

newRRHO <- function (self, ...)
{
    UseMethod("newRRHO")
}

newRRHO.data.frame <- function (self)
{
    if ( ! "a" %in% colnames(self))
        stop("Column 'a' is missing!")
    if ( ! "b" %in% colnames(self))
        stop("Column 'b' is missing!")
    
    structure(
        list(data = self),
        class = "rrho"
    )
}

newRRHO.numeric <- function (a, b)
{
    if (length(a) != length(b))
        stop("'a' and 'b' parameters should be of same length!")
    newRRHO(data.frame(a=a, b=b))
}

rrho_plot <- function (a,b, n.dots=500)
{
    n <- length(a)
    if (n.dots > n)
        n.dots  <- n
    rrho <- rrho_rectangle(1,1, n, n, n.dots, n.dots, a, b, LOG=TRUE)


    ggplot2::ggplot(reshape2::melt(rrho),  ggplot2::aes(Var1,Var2, fill=value)) +  ggplot2::geom_raster() +
         ggplot2::scale_fill_gradientn(colours=c('#021893', "#3537ae", "#740699", "#b70b0b", "#990623"))
}
