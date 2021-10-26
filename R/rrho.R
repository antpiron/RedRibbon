### S3 Methods RedRibbon

#' RedRibbon generic
#' 
#' TODO: Description
#' @export
RedRibbon <- function (self, ...)
{
    UseMethod("RedRibbon")
}

#' setoptions generic
#' 
#' TODO: Description
#' @export
setoptions <- function (self, ...)
{
    UseMethod("setoptions")
}

#' quadrants generic
#' 
#' TODO: Description
#' @export
quadrants <- function (self, ...)
{
    UseMethod("quadrants")
}

#' rectangle_min generic
#' 
#' TODO: Description
#' @export
rectangle_min <- function (self, ...)
{
    UseMethod("rectangle_min")
}

#' permutation generic
#' 
#' TODO: Description
#' @export
permutation <- function (self, ...)
{
    UseMethod("permutation")
}

#' enrichment generic
#' 
#' TODO: Description
#' @export
enrichment <- function (self, ...)
{
    UseMethod("enrichment")
}

### S3 Body

#' Creates a RedRibbon object from 2 numeric vectors
#' 
#' TODO: Description
#' @param df a data.frame. The real number columns a and b are mandatory.
#' @param enrichment_mode 
#'    \itemize{
#'    \item{"hyper"} {for one tailed hypergeometric test}
#'    \item{"hyper-two-tailed"} {for one tailed hypergeometric test}
#'    \item{"hyper-two-tailed-old"} {for the original R package two tailed test.}
#' }
#' @param correlation the correlation vector (see newFC function).
#' @return A RedRibbon S3 object.
#' 
#' @examples
#' 
#' library(RedRibbon)
#' rr <- RedRibbon(data.frame(id=c("gene1", "gene2", "gene3", "gene4"),
#'                            a=c(0.5, 0.7,0.3, 0.8),
#'                            b=c(0.6,0.6,0.4,0.7))
#' 
#' @method RedRibbon data.frame
#' @export
RedRibbon.data.frame <- function (df, enrichment_mode=c("hyper", "hyper-two-tailed", "hyper-two-tailed-old"), correlation=NULL)
{
    if ( ! "a" %in% colnames(df))
        stop("Column 'a' is missing!")
    if ( ! "b" %in% colnames(df))
        stop("Column 'b' is missing!")

    colfunc <- grDevices::colorRampPalette(c("#eb3434", "#eb9334", "#ebeb34", "#49eb34", "#34eba5", "#34b4eb", "#3446eb"))
    colors  <-  colfunc(1000)
    
    structure(
        list(data = df,
             enrichment_mode = ifelse(is.null( enrichment_mode), c("hyper"), enrichment_mode[1]),
             correlation=correlation,
             ggplot_colours = c(colors, rev(colors))
             ),
        class = "rrho"
    )
}

#' Creates a RedRibbon objects from 2 numeric vectors
#' 
#' A helper function to check that \code{a} and \code{b} input parameters are of the same length.
#' The function is equivalent to RedRibbon(data.frame(a=a, b=b), ...) (see ).
#' 
#' @param a is a vector of double.
#' @param b is a vector of double.
#' @return A rrho S3 object.
#' 
#' @examples
#' 
#' library(RedRibbon)
#' rr <- RedRibbon(c(0.5, 0.7,0.3, 0.8), c(0.6,0.6,0.4,0.7))
#' 
#' @method RedRibbon numeric
#' @export
RedRibbon.numeric <- function (a, b, ...)
{
    if (length(a) != length(b))
        stop("'a' and 'b' parameters should be of same length!")
    RedRibbon(data.frame(a=a, b=b), ...)
}

#' Set RedRibbon options on a RedRibbon S3 object
#' 
#' Helper function allowing to change the enrichment method that you wish to use as well as the color scheme
#' for the RedRibbon level map.
#' 
#' @param self A RedRibbon object.
#' @param enrichment_mode A string (default = NULL). 
#'    \itemize{
#'    \item{"enrichment"} {for one tailed hypergeometric test}
#'    \item{"hyper-two-tailed"} {for one tailed hypergeometric test, "hyper-two-tailed-old", for the original R package two tailed test.}
#' }
#' [TODO] Describe pros and cons of each method (time, efficacity, ...), what happens is there is no method specified? Set a default?
#' @param ggplot_colours is the color palette used for the plots. The default is
#'    \code{
#'    colfunc <- grDevices::colorRampPalette(c("#eb3434", "#eb9334", "#ebeb34", "#49eb34", "#34eba5", "#34b4eb", "#3446eb"))
#'    colors  <-  colfunc(1000)
#'    ggplot_colours = c(colors, rev(colors))
#'    }
#' @return A RedRibbon S3 object with updated parameters.
#' @examples
#' library(magrittr)
#' library(RedRibbon)
#' RedRibbon(c(0.5, 0.7,0.3, 0.8), c(0.6,0.6,0.4,0.7)) %>% 
#'  setoptions(enrichment_mode="hyper-two-tailed")
#' @method setoptions rrho
#' @export
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
#' TODO: Description
#' @param m is the number of coordinates to compute on the y axis (b)
#' @param n is the number of coordinates to compute on the x axis (a)
#' @param whole if TRUE run the whole list otherwise run by quadrants.
#' @param whole.fraction is the fraction of the quadrant analysed in `whole` mode.
#' @param algorithm is the algorithm used to find the minimal p-value: 
#' \itemize{
#' 
#' \item{"classic"} {Naive research} 
#' \item{"ea"} {Evolutionary algorithm, faster but }.
#' }
#' @param permutation is TRUE if the permutation mode is run.
#' @param niter is the number of iteration for the permutation mode.
#' @return A list of coordinates, one by quadrant.
#' @examples
#' library(RedRibbon)
#' 
#' a <- as.double(1:1000) - 450
#' b <- as.double(1:1000) - 460
#' rr <- RedRibbon(a, b)
#' 
#' quad <- quadrants(rr, m=1000, n=1000)
#' @method quadrants rrho
#' @export
quadrants.rrho <- function(self, m=NULL, n=NULL,
                           whole=TRUE, whole.fraction = 1, algorithm="classic", permutation=FALSE, niter=96)
{
    len <- length(self$data$a)

    do.quadrant <- function (i, j, i.len, j.len, direction="enrichment", directions="downdown")
    # [TODO] direction = "enrichment?" So we do not use the parameter?
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
        len.fraction <- min(len * whole.fraction, len)
        quadrant <- do.quadrant(1, 1, len.fraction, len.fraction, direction="enrichment", directions="downdown")
        if (! all(is.na(quadrant)) )
            quadrants$whole <- quadrant
    } 
    else
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

#' Render the RRHO map.
#' 
#' You can choose to render the RedRibbon level map using \code{ggplot2}. 
#' 
#' @param self is the RedRibbon object
#' @param n is the number of coordinates to compute on the x and y axis (Default = sqrt(len))
#' @param repel.force is the value of the repel force for the p-value ploting (default = 150)
#' @param base_size is the size of the text fields (default = 20)
#' @return A \code{ggplot} object.
#' @method ggplot rrho
#' @export
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
        ggplot2::xlab(labels[1]) + ylab(labels[2]) +
        ## scale_x_continuous(labels = label_percent(accuracy = 1, scale = 100/n.i)) +
        ## scale_y_continuous(labels = label_percent(accuracy = 1, scale = 100/n.j) ) +
        ggplot2::scale_x_continuous(breaks = c(0 + n * 0.1, n - n * 0.1), labels = c("down", "up"), expand = c(0, 0)) +
        ggplot2::scale_y_continuous(breaks = c(0 + n * 0.1, n - n * 0.1), labels = c("down", "up"), expand = c(0, 0)) +
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

#' rectangle_min
#' 
#' TODO: Description
#' 
#' @export
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

#' permutation
#' 
#' TODO: Description
#' 
#' @export
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

#' enrichment
#' 
#' TODO: Description
#' 
#' @export
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

#' newLDFIT
#' 
#' TODO: Description
#' 
#' @export
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

#' Create a fold change prediction object
#'
#' 
#' @param deps is a vector of integer referring to the feature used for the predicton.
#'             The values should be either -1 for an unpredicted features or in 1:length(deps).
#'             
#' @param beta is a vector of double. The fold change will be predicted as FCy = beta * FCx.
#' 
#' @export
newFC  <- function (deps, beta)
{
    structure(
        list(
            deps = as.integer(deps),
            beta = as.double(beta)
        ),
        class = "fc"
    )
}


#' Drop in replacement for original R function
#'
#' `RRHO` function preserve the compatibility with the original package but
#' with the performance and accuracy of the new method. The stepsize parameter is
#' used only to return the hypermat and ignored to determine the minimal p-values
#' in the four quadrants as the evolutionary algorithm is used.
#' 
#' @export
RRHO  <- function (list1, list2,
                   stepsize = NULL,
                   labels= c("a", "b"),
                   alternative=c('two.sided', 'enrichment'),
                   plots = FALSE,
                   outputdir = NULL,
                   BY = FALSE,
                   log10.ind=FALSE)
{

    warning('This function only exists for compatibility. Please use instead the RedRibbon() call. See Vignette("RedRibbon") for details.')
    if ( ! is.data.frame(list1) )
        stop("list1 is not a data.frame")
    
    if ( ! is.data.frame(list2) )
        stop("list2 is not a data.frame")
    
    if(! alternative[1] %in% c('two.sided', 'enrichment') )
        stop('Wrong alternative specified should be either `two.sided` or `enrichment`.')

    dt1 <- data.table(list1[,1:2])
    colnames(dt1) <- c("id", "a")
    dt2 <- data.table(list2[,1:2])
    colnames(dt2) <- c("id", "b")
    
    
    dt <- merge(dt1, dt2, by="id")

    if ( is.null(stepsize) )
        stepsize <- floor(sqrt(nrow(dt)))

    enrichment_mode  <- if (alternative[1] == "two.sided") "hyper-two-tailed" else "hyper"
    rr <- RedRibbon(dt, enrichment_mode=enrichment_mode)
    quad <- quadrants(rr, algorithm="ea", permutation=TRUE, whole=FALSE)

    result  <- list(rr = rr, quadrants = quad )
    len <- nrow(dt)
    n.i <- len %/% stepsize
    n.j <- len %/% stepsize
    result$hypermat <- rrho_rectangle(1, 1, len, len, n.i, n.j, rr$data$a, rr$data$b,  mode=rr$enrichment_mode, LOG=TRUE)

    
    if (! is.null(outputdir) )
        dir.create(outputdir, showWarnings = FALSE, recursive = TRUE)

    if (plots)
    {
        result$gg <- ggplot(rr, quadrants=quad) + coord_fixed(ratio = 1)


        if (! is.null(outputdir) )
        {
            gg_fn <-file.path(outputdir, paste0("RRHOMap", labels[1], "_VS_", labels[2], ".jpg") )
            ggsave(gg_fn, gg, width=8, height=8, units="in", quality=100, dpi=300)
        }
        
    }

    ## Write out the gene lists of overlapping
    if (! is.null(outputdir) )
    {
        write.overlap <- function (fn, direction="downdown")
        {
            if ( ! is.null(quad[[direction]]) )
            {
                ids <- dt[ quad[[direction]]$positions, ]$id
                fwrite.table(ids, fn)
            }
        }

        dir.create(outputdir, showWarnings = FALSE, recursive = TRUE)
        
        write.overlap(file.path(outputdir, paste0("RRHO_GO_MostDownregulated", labels[1], "_VS_", labels[2], ".csv")),
                      "downdown")
        write.overlap(file.path(outputdir, paste0("RRHO_GO_MostUpregulated", labels[1], "_VS_", labels[2], ".csv")),
                      "upup")
        write.overlap(file.path(outputdir, paste0("RRHO_GO_MostDownUpregulated", labels[1], "_VS_", labels[2], ".csv")),
                      "downup")
        write.overlap(file.path(outputdir, paste0("RRHO_GO_MostUpDownregulated", labels[1], "_VS_", labels[2], ".csv")),
                      "updown")   
    }

    return (result)
}
