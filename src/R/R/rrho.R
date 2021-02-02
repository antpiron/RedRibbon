

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


###############################
### S3
###

### S3 Methods

newRRHO <- function (self, ...)
{
    UseMethod("newRRHO")
}

enrichment_mode <- function (self, ...)
{
    UseMethod("enrichment_mode")
}

### S3 Body

newRRHO.data.frame <- function (df)
{
    if ( ! "a" %in% colnames(df))
        stop("Column 'a' is missing!")
    if ( ! "b" %in% colnames(df))
        stop("Column 'b' is missing!")
    
    structure(
        list(data = df,
             enrichment_mode = c("hyper"),
             ggplot_colours = c('#021893', "#3537ae", "#740699", "#b70b0b", "#990623")
             ),
        class = "rrho"
    )
}

newRRHO.numeric <- function (a, b)
{
    if (length(a) != length(b))
        stop("'a' and 'b' parameters should be of same length!")
    newRRHO(data.frame(a=a, b=b))
}

enrichment_mode.character <- function(self, mode)
{
    self$enrichment_mode <- mode
    
    return(self)
}

ggplot.rrho <- function (self, n = NULL)
{
    len <- length(self.a)

    if ( is.null(n) )
        n <- max(sqrt(len), 300)
            
    n.i <- n
    n.j <- n

    rrho <- rrho_rectangle(1,1, len, len, n.i, n.j, self$data$a, self$data$b, LOG=TRUE)

    self$gg <-  ggplot2::ggplot(reshape2::melt(rrho),  ggplot2::aes(Var1,Var2, fill=value)) +  ggplot2::geom_raster() +
        ggplot2::scale_fill_gradientn(colours=self$ggplot_colours)
    
    return(self)
}

