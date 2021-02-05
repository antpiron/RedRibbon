

## SEXP rrho_r_rectangle(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP m, SEXP n, SEXP a, SEXP b, SEXP mode, SEXP log_flag)

rrho_rectangle  <- function (i, j, i.len, j.len, m, n, a, b, mode=c("hyper"), LOG=FALSE)
{
    .Call("rrho_r_rectangle", as.integer(i), as.integer(j), as.integer(i.len), as.integer(j.len),
          as.integer(m), as.integer(n), as.double(a), as.double(b), as.character(mode), as.logical(LOG))
}

rrho_rectangle_min  <- function (i, j, i.len, j.len, m, n, a, b, mode=c("hyper"), direction="enrichment")
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

## SEXP rrho_r_intersect(SEXP i, SEXP j, SEXP a, SEXP b, SEXP directions)
rrho_intersect  <- function (i, j, a, b, directions=c("downdown"))
{
    .Call("rrho_r_intersect", as.integer(i), as.integer(j), as.double(a), as.double(b), as.character(directions))
}


###############################
### S3
###

### S3 Methods

newRRHO <- function (self, ...)
{
    UseMethod("newRRHO")
}

setoptions <- function (self, ...)
{
    UseMethod("setoptions")
}

rectangle_min <- function (self, ...)
{
    UseMethod("rectangle_min")
}

enrichment <- function (self, ...)
{
    UseMethod("enrichment")
}

### S3 Body

newRRHO.data.frame <- function (df)
{
    if ( ! "a" %in% colnames(df))
        stop("Column 'a' is missing!")
    if ( ! "b" %in% colnames(df))
        stop("Column 'b' is missing!")

    colfunc <- colorRampPalette(c("#eb3434", "#eb9334", "#ebeb34", "#49eb34", "#34eba5", "#34b4eb", "#3446eb"))
    colors  <-  colfunc(1000)
    
    structure(
        list(data = df,
             enrichment_mode = c("hyper"),
             ggplot_colours = c(colors, rev(colors)),
             draw_quadrants = FALSE,
             draw_minimal_pvalue = FALSE
             ),
        class = "rrho"
    )
}

#' Create a rrho S3 object from two vectors of values
#' 
#' @param a A vector of float.
#' @param y A vector of float.
#' @return A rrho S3 object.
#' @examples
#' newRRHO(c(0.5, 0.7,0.3, 0.8), c(0.6,0.6,0.4,0.7))
newRRHO.numeric <- function (a, b)
{
    if (length(a) != length(b))
        stop("'a' and 'b' parameters should be of same length!")
    newRRHO(data.frame(a=a, b=b))
}

setoptions.rrho <- function(self, enrichment_mode=NULL, ggplot_colours = NULL, draw_quadrants = NULL,  draw_minimal_pvalue = NULL, labels = NULL)
{
    if (! is.null(enrichment_mode) )
        self$enrichment_mode <- enrichment_mode

    if (! is.null(ggplot_colours) )
        self$ggplot_colours <- ggplot_colours

    if (! is.null(draw_quadrants) )
        self$draw_quadrants <- draw_quadrants

    if (! is.null(draw_minimal_pvalue) )
        self$draw_minimal_pvalue <- draw_minimal_pvalue

    if (! is.null(labels) )
        self$labels <- labels
    
    return(self)
}



ggplot.rrho <- function (self, n = NULL, labels = c("a", "b"))
{
    len <- length(self$data$a)

    if ( is.null(n) )
        n <- max(sqrt(len), 500)
            
    n.i <- n
    n.j <- n

    rrho <- rrho_rectangle(1, 1, len, len, n.i, n.j, self$data$a, self$data$b,  mode=self$enrichment_mode, LOG=TRUE)

    max.log <- max(abs(rrho))
    if (0 == max.log)
    {
        min.log  <- -0.001
        max.log <- 0.001
    } else
        min.log <- - max.log
    ticks <- c(min.log, 0, max.log)

    len.colors <- length(self$ggplot_colours)
    half.len.colors <- len.colors %/% 2
    colors.values <- seq(0, len.colors) /  len.colors
    gg <-  ggplot2::ggplot(reshape2::melt(rrho),  ggplot2::aes(Var1,Var2, fill=value)) +
        ggplot2::geom_raster() +
        ## ggplot2::scale_fill_gradientn(colours=self$ggplot_colours, name="-log p.val") +
        ggplot2::scale_fill_gradientn(colors = self$ggplot_colours, breaks = ticks,
                                      labels = format(ticks),
                                      limits=ticks[c(1,3)],
                                      ##limits=b[c(1,length(colors))],
                                      name="-log p.val",
                                      values=colors.values) +
        xlab(labels[1]) + ylab(labels[2]) +
        scale_x_continuous(labels = label_percent(accuracy = 1, scale = 100/n.i)) +
        scale_y_continuous(labels = label_percent(accuracy = 1, scale = 100/n.j) )
    
    return(gg)
}

rectangle_min.rrho <- function(self, i, j, i.len, j.len, m=NULL, n=NULL, direction="enrichment")
{
    len <- length(self$data$a)
   
    if ( i < 1 || i > len )
        stop("i outside range [1, n]")
    if ( j < 1 || j > len )
        stop("j outside range [1, n]")

    ri <- i + i.len - 1
    if ( ri < 1 || ri > len )
        stop("i + i.len - 1 outside range [1, n]")
    rj <- j + j.len - 1
    if ( rj < 1 || rj > len )
        stop("j + j.len - 1 outside range [1, n]")

    
    len <- length(self$data$a)
    
    if ( is.null(m) )
        m <- max(sqrt(len), 500)

    if ( is.null(n) )
        n <- max(sqrt(len), 500)
            
    result <- rrho_rectangle_min(i, j, i.len, j.len, m, n, self$data$a, self$data$b, mode=self$enrichment_mode, direction=direction)

    return(result)
}

enrichment.rrho <- function(self, i, j, directions="downdown")
{
    len <- length(self$data$a)
   
    if ( i < 1 || i > len )
        stop("i outside range [1, n]")
    if ( j < 1 || j > len )
        stop("j outside range [1, n]")
    
    return(rrho_intersect(i, j, self$data$a, self$data$b, directions))
}
