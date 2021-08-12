

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


## SEXP rrho_r_permutation(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP a, SEXP b, SEXP algo_params,
##		   SEXP mode, SEXP direction, SEXP algorithm,
##		   SEXP correlation,
##		   SEXP niter, SEXP pvalue_i, SEXP pvalue_j)
rrho_permutation <- function (i, j, i.len, j.len, a, b, algo_params=NULL, mode=c("hyper"), direction="enrichment", algorithm="classic", correlation=NULL,
                              niter=96, pvalue_i, pvalue_j)
{
    if ( is.null( algo_params ) )
        algo_params <- list()

    if ( is.null(algo_params[["m"]]) )
        algo_params[["m"]] <- as.integer(i.len)
    else
        algo_params[["m"]] <- as.integer(algo_params[["m"]])
    if ( is.null(algo_params[["n"]]) )
        algo_params[["n"]] <- as.integer(j.len)
    else
        algo_params[["n"]] <- as.integer(algo_params[["n"]])

    if (! is.null(correlation) && class(correlation) != "ld_fit")
        stop("Correlation should be NULL or of `ld_fit` class")
    
    .Call("rrho_r_permutation", as.integer(i), as.integer(j), as.integer(i.len), as.integer(j.len),
          as.double(a), as.double(b), algo_params, as.character(mode), as.character(direction),
          as.character(algorithm), correlation, as.integer(niter), as.integer(pvalue_i), as.integer(pvalue_j))
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

### S3 Methods RRHO

#' Create a rrho S3 object.
#' 
#' @return A rrho S3 object.
#' @examples
#' newRRHO(c(0.5, 0.7,0.3, 0.8),
#'         c(0.6, 0.6, 0.4, 0.7))
#' newRRHO(data.frame(a = c(0.5, 0.7, 0.3, 0.8),
#'                    b = c(0.6, 0.6, 0.4, 0.7)))
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

permutation <- function (self, ...)
{
    UseMethod("permutation")
}

enrichment <- function (self, ...)
{
    UseMethod("enrichment")
}

### S3 Body

newRRHO.data.frame <- function (df, enrichment_mode=NULL, correlation=NULL)
{
    if ( ! "a" %in% colnames(df))
        stop("Column 'a' is missing!")
    if ( ! "b" %in% colnames(df))
        stop("Column 'b' is missing!")

    colfunc <- grDevices::colorRampPalette(c("#eb3434", "#eb9334", "#ebeb34", "#49eb34", "#34eba5", "#34b4eb", "#3446eb"))
    colors  <-  colfunc(1000)
    
    structure(
        list(data = df,
             enrichment_mode = ifelse(is.null( enrichment_mode), c("hyper"), enrichment_mode[[1]]),
             correlation=correlation,
             ggplot_colours = c(colors, rev(colors))
             ),
        class = "rrho"
    )
}

#' Create a rrho S3 object from two vectors of values
#' 
#' @param a is a vector of double.
#' @param b is a vector of double.
#' @return A rrho S3 object.
#' @examples
#' newRRHO(c(0.5, 0.7,0.3, 0.8), c(0.6,0.6,0.4,0.7))
newRRHO.numeric <- function (a, b, ...)
{
    if (length(a) != length(b))
        stop("'a' and 'b' parameters should be of same length!")
    newRRHO(data.frame(a=a, b=b), ...)
}

#' Set rrho options a rrho S3 object from two vectors of values
#' 
#' @param self is a rrho object.
#' @param enrichment_mode A string. "enrichment" for one tailed hypergeometric test,
#'    "hyper-two-tailed" for one tailed hypergeometric test, "hyper-two-tailed-old", for
#'    the original R package two tailed test.
#' @param ggplot_colours is the color palette used for the plots. The default is
#'    \code{
#'    colfunc <- grDevices::colorRampPalette(c("#eb3434", "#eb9334", "#ebeb34", "#49eb34", "#34eba5", "#34b4eb", "#3446eb"))
#'    colors  <-  colfunc(1000)
#'    ggplot_colours = c(colors, rev(colors))
#'    }
#' @return The updated rrho S3 object.
#' @examples
#' library(magrittr)
#' newRRHO(c(0.5, 0.7,0.3, 0.8), c(0.6,0.6,0.4,0.7)) %>% setoptions(enrichment_mode="hyper-two-tailed",  draw_quadrants = TRUE)
setoptions.rrho <- function(self, enrichment_mode=NULL, ggplot_colours = NULL)
{
    if (! is.null(enrichment_mode) )
        self$enrichment_mode <- enrichment_mode

    if (! is.null(ggplot_colours) )
        self$ggplot_colours <- ggplot_colours
    
    return(self)
}

#' Compute the best coordinates.
#' 
#' @param m is the number of coordinates to compute on the y axis (b)
#' @param n is the number of coordinates to compute on the x axis (a)
#' @param whole if TRUE run the whole list otherwise run by quadrants.
#' @param algorithm is the algorithm used to find the minimal p-value: "classic" or "ea" (evolutionary algorithm).
#' @param permutation is TRUE if the permutation mode is run.
#' @param niter is the number of iteration for the permutation mode.
#' @return A rrho S3 object.
#' @examples
#' a <- as.double(1:1000) - 450
#' b <- as.double(1:1000) - 460
#' 
#' rr <- newRRHO(a, b)
#' 
#' quad <- quadrants(rr, m=1000, n=1000)
quadrants.rrho <- function(self, m=NULL, n=NULL,
                           whole=TRUE, algorithm="classic", permutation=FALSE, niter=96)
{
    len <- length(self$data$a)

    do.quadrant <- function (i, j, i.len, j.len, direction="enrichment", directions="downdown")
    {
        ret  <- NA
        if ( is.null(m) )
           m  <- ifelse(sqrt(i.len) < 500, 500, sqrt(i.len) )
        if ( is.null(n) )
           n  <- ifelse(sqrt(j.len) < 500, 500, sqrt(j.len) )
        coord <- rectangle_min(self, i, j, i.len, j.len, m=m, n=n, direction=direction, algorithm=algorithm)
        if (! all(is.na(coord)) )
        {
            enrich.ret  <-  enrichment(self, coord[1], coord[2], directions=directions)
            direction.integer <- if (direction == "enrichment") 1 else -1
            if (enrich.ret$direction == direction.integer)
            {
                ret <- enrich.ret
                ret$i  <- coord[1]
                ret$j  <- coord[2]
                if (permutation)
                {
                    perm <- permutation(self, i, j, i.len, j.len, self$data$a, self$data$b, algo_params=list(m=m, n=n),
                                        direction="enrichment", algorithm=algorithm, correlation=self$correlation, niter=niter, pvalue_i=ret$i, pvalue_j=ret$j)
                    ret$padj <- perm$pvalue
                    ret$log_padj <- perm$log_pvalue
                }
            }
        }

        return(ret);
    }

    quadrants = list()
    
    if (whole)
    {
        quadrant <- do.quadrant(1, 1, len, len, direction="enrichment", directions="downdown")
        if (! all(is.na(quadrant)) )
            quadrants$whole <- quadrant
    } else
    {
        a_ltzero <- sum(self$data$a < 0)
        b_ltzero <- sum(self$data$b < 0)

        quadrant <- do.quadrant(1, 1, a_ltzero, b_ltzero, direction="enrichment", directions="downdown")
        if (! all(is.na(quadrant)) )
            quadrants$downdown <- quadrant
        
        quadrant <- do.quadrant( a_ltzero + 1, b_ltzero + 1, len - a_ltzero, len - b_ltzero, direction="enrichment", directions="upup")
        if (! all(is.na(quadrant)) )
            quadrants$upup <- quadrant
        

        quadrant <- do.quadrant(a_ltzero + 1, 1, len - a_ltzero, b_ltzero, direction="notenrichment", directions="updown")
        if (! all(is.na(quadrant)) )
            quadrants$updown <- quadrant
        

        quadrant <- do.quadrant(1, b_ltzero + 1, a_ltzero, len - b_ltzero, direction="notenrichment", directions="downup")
        if (! all(is.na(quadrant)) )
            quadrants$downup <- quadrant
    }

    return(quadrants)
}

#' Compute the RRHO map.
#' 
#' @param n is the number of coordinates to compute on the x and y axis
#' @param repel.force is the value of the repel force for the p-value ploting
#' @param base_size is the size of the text fields
#' @return A ggplot object.
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
        ## scale_x_continuous(labels = label_percent(accuracy = 1, scale = 100/n.i)) +
        ## scale_y_continuous(labels = label_percent(accuracy = 1, scale = 100/n.j) ) +
        scale_x_continuous(breaks = c(0 + n * 0.1, n - n * 0.1), labels = c("down", "up"), expand = c(0, 0)) +
        scale_y_continuous(breaks = c(0 + n * 0.1, n - n * 0.1), labels = c("down", "up"), expand = c(0, 0)) +
        ## ggplot2::theme_bw() +
        ggplot2::theme(axis.title=element_text(size=base_size,face="bold"),
                       legend.title = element_text(size = base_size * 7 / 10),
                       legend.text = element_text(size = base_size * 1 / 2),
                       ) +
        ggplot2::theme(axis.text.x=ggplot2::element_text(size=base_size* 7 / 10, face="bold"),
                       axis.ticks.x=ggplot2::element_blank(),
                       axis.text.y=ggplot2::element_text(size=base_size * 7 / 10, face="bold", angle=90),
                       axis.ticks.y=ggplot2::element_blank(),
                       axis.ticks.length = unit(0, "pt"),
                       panel.grid.major = ggplot2::element_blank(),
                       panel.grid.minor = ggplot2::element_blank(), 
                       panel.spacing = unit(0, "cm"),
                       plot.margin = margin(0, 0, 0, 0, "cm"))
        

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
                                          function (quadrant)
                                          {
                                              if ( quadrant$pvalue > 0.05 || (! is.null(quadrant$padj) && quadrant$padj > 0.05) )
                                                  return(NULL)
                                              
                                              pvalue <- quadrant$log_pvalue
                                              pvalue.formatted <-  formatC(pvalue, format = "f", digits = 1)
                                              
                                              if (! is.null(quadrant$padj) )
                                              {
                                                  padj <- quadrant$log_padj
                                                  pvalue.formatted <-  paste(pvalue.formatted,
                                                                             "(padj =", formatC(padj, format = "f", digits = 1), ")")
                                              }
                                              data.frame(i=quadrant$i, j=quadrant$j,
                                                         pvalue=pvalue.formatted, value=pvalue)
                                          })))
                
                if ( nrow(quadrants_df) > 0 )
                    gg <- gg +
                        ggrepel::geom_text_repel(data=quadrants_df,
                                                 aes(x=i * n.i / len, y=j * n.j / len,
                                                     label=pvalue,
                                                     colour = "gray"),
                                                 hjust=1, vjust=1, colour = "black",
                                                 force = repel.force, show.legend = FALSE, size = pval_size)

            }
        }
    }
    
    return(gg)
}

rectangle_min.rrho <- function(self, i, j, i.len, j.len, m=NULL, n=NULL, direction="enrichment", algorithm="classic")
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

        

    if ("classic" != algorithm)
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

permutation.rrho <- function (self, i, j, i.len, j.len, a, b, algo_params=NULL, direction="enrichment", algorithm="classic", correlation=NULL, niter=96, pvalue_i, pvalue_j)
{
    if ( is.null( algo_params ) )
        algo_params <- list()
    
    if ( is.null(algo_params[["m"]]) )
        algo_params[["m"]] <- as.integer(sqrt(i.len))
    if ( is.null(algo_params[["n"]]) )
        algo_params[["n"]] <- as.integer(sqrt(j.len))

    rrho_permutation(i, j, i.len, j.len, a, b, algo_params=algo_params, mode=self$enrichment_mode, direction=direction, algorithm=algorithm,
                     correlation=correlation, niter=niter, pvalue_i=pvalue_i, pvalue_j=pvalue_j)
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

newLDFIT  <- function (position, deps, half = 6480.306)
{
    structure(
        list(half = as.double(half),
             pos = as.integer(position),
             deps = as.integer(deps)
             ),
        class = "ld_fit"
    )
}
