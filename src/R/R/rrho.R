

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

quadrants <- function (self, ...)
{
    UseMethod("quadrants")
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

    colfunc <- grDevices::colorRampPalette(c("#eb3434", "#eb9334", "#ebeb34", "#49eb34", "#34eba5", "#34b4eb", "#3446eb"))
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

quadrants.rrho <- function(self, m=NULL, n=NULL,
                           whole=TRUE, ea=FALSE, threshold=0.05)
{
    len <- length(self$data$a)

    quadrants = list()
    
    if (whole)
    {
        coord <- rectangle_min(self, 1, 1, len, len, m=m, n=n, direction="enrichment", ea=ea)
        if (! all(is.na(coord)) )
        {
            
            quadrants$whole <- enrichment(self, coord[1], coord[2])
            quadrants$whole$i  <- coord[1]
            quadrants$whole$j  <- coord[2]
        }
    } else
    {
        a_ltzero <- sum(self$data$a < 0)
        b_ltzero <- sum(self$data$b < 0)

        coord_downdown <- rectangle_min(self, 1, 1, a_ltzero, b_ltzero, m=m, n=n, direction="enrichment", ea=ea)
        if (! all(is.na(coord_downdown)) )
        {
            downdown <- enrichment(self, coord_downdown[1], coord_downdown[2])
            if (downdown$direction > 0 && downdown$pvalue <= threshold)
            {
                quadrants$downdown <- enrichment(self, coord_downdown[1], coord_downdown[2])
                quadrants$downdown$i  <- coord_downdown[1]
                quadrants$downdown$j  <- coord_downdown[2]
            }
        }

        coord_upup <- rectangle_min(self, a_ltzero + 1, b_ltzero + 1, len - a_ltzero, len - b_ltzero,
                                    m=m, n=n, direction="enrichment", ea=ea)
        if (! all(is.na(coord_upup)) )
        {
            upup <- enrichment(self, coord_upup[1], coord_upup[2], directions="upup")
            if (upup$direction > 0 && upup$pvalue <= threshold)
            {
                quadrants$upup <- upup
                quadrants$upup$i  <- coord_upup[1]
                quadrants$upup$j  <- coord_upup[2]
            }
        }

        coord_updown <- rectangle_min(self, a_ltzero + 1, 1, len - a_ltzero, b_ltzero,
                                    m=m, n=n, direction="notenrichment", ea=ea)
        if (! all(is.na(coord_updown)) )
        {
            updown <- enrichment(self, coord_updown[1], coord_updown[2], directions="updown")
            if (updown$direction < 0 && updown$pvalue <= threshold)
            {
                quadrants$updown <- updown
                quadrants$updown$i  <- coord_updown[1]
                quadrants$updown$j  <- coord_updown[2]
            }
        }

        coord_downup <- rectangle_min(self, 1, b_ltzero + 1, a_ltzero, len - b_ltzero,
                                    m=m, n=n, direction="notenrichment", ea=ea)
        if (! all(is.na(coord_downup)) )
        {
            downup <- enrichment(self, coord_downup[1], coord_downup[2], directions="downup")
            if (downup$direction < 0 && downup$pvalue <= threshold)
            {
                quadrants$downup <-downup
                quadrants$downup$i  <- coord_downup[1]
                quadrants$downup$j  <- coord_downup[2]
            }
        }

    }

    return(quadrants)
}

ggplot.rrho <- function (self, n = NULL, labels = c("a", "b"), show.quadrants=TRUE, quadrants=NULL, show.pval=TRUE,
                         repel.force=150, base_size=20)
{
    len <- length(self$data$a)

    if ( is.null(n) )
        n <- min(max(sqrt(len), 500), len)
            
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

    ## find the middle of the plots
    if (show.quadrants || show.pval)
    {
        a_ltzero <- sum(self$data$a < 0)
        x.ind <- a_ltzero
        if ( x.ind == 0 )
            x.ind = len / 2
        
        b_ltzero <- sum(self$data$b < 0)
        y.ind <- b_ltzero
        if ( y.ind == 0 )
            y.ind = len / 2
        
        ## plot dotted quadrant lines
        if (show.quadrants)
        {
            print(x.ind)
            print( n.i)
            print(n)
            print(x.ind * n.i / n)
            gg  <- gg +
                ggplot2::geom_vline(aes(xintercept = x.ind * n.i / len), 
                                    linetype = "dotted", colour = "gray10",size = 1) +
                ggplot2::geom_hline(aes(yintercept = y.ind * n.j / len), 
                                    linetype = "dotted", colour = "gray10",size = 1)
        }
        
        ## plot pvalue
        if (show.pval)
        {
            if (! is.null(quadrants) )
            {
            pval_size  <- as.integer(base_size * 1/5)
            quadrants_df <- as.data.frame(
                do.call(rbind, lapply(quadrants,
                                      function (quadrant) data.frame(i=quadrant$i, j=quadrant$i,
                                                                     pvalue=quadrant$pvalue, value=quadrant$pvalue))))
            print(quadrants_df)
            gg <- gg +
                ggrepel::geom_text_repel(data=quadrants_df,
                                         aes(x=i * n.i / len, y=j * n.j / len,
                                             label=formatC(pvalue,
                                                           format = "e", digits = 1),
                                             colour = "gray"),
                                         hjust=1, vjust=1, colour = "black",
                                         force = repel.force, show.legend = FALSE, size = pval_size)
            }
        }
    }
    
    return(gg)
}

rectangle_min.rrho <- function(self, i, j, i.len, j.len, m=NULL, n=NULL, direction="enrichment", ea=FALSE)
{
    len <- length(self$data$a)
   
    if ( i < 1 || i > len )
        stop("i outside range [1, n]")
    if ( j < 1 || j > len )
        stop("j outside range [1, n]")

    ri <- i + i.len - 1
    if ( ri < 1 || ri > len )
        stop(paste0("i + i.len = ", i, " + ", i.len, " outside range [1, n]"))
    rj <- j + j.len - 1
    if ( rj < 1 || rj > len )
        stop("j + j.len outside range [1, n]")

        

    if (ea)
    {
        result <- rrho_rectangle_min_ea(i, j, i.len, j.len,
                                        self$data$a, self$data$b,
                                        mode=self$enrichment_mode, direction=direction)
    } else
    {
        if ( is.null(m) )
            m <- max(sqrt(len), 500)

        if ( is.null(n) )
            n <- max(sqrt(len), 500)
        m <- min(m, i.len)
        n <- min(n, j.len)

        result <- rrho_rectangle_min(i, j, i.len, j.len, m, n,
                                     self$data$a, self$data$b,
                                     mode=self$enrichment_mode, direction=direction)
    }

    return(result)
}


enrichment.rrho <- function(self, i, j, directions="downdown")
{
    len <- length(self$data$a)
   
    if ( i < 1 || i > len )
        stop("i outside range [1, n]")
    if ( j < 1 || j > len )
        stop("j outside range [1, n]")

    res <- rrho_ij(i, j, self$data$a, self$data$b, mode=self$enrichment_mode)
    res$positions <- rrho_intersect(i, j, self$data$a, self$data$b, directions)
    
    return(res)
}
