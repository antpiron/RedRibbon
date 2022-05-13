## SEXP rrho_r_rectangle(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP m, SEXP n, SEXP a, SEXP b, SEXP mode, SEXP log_flag)
#' Call to C function (internal use): compute the full grid rectangle
#'
#' @param i is the y coordinates of the rectangle (in the `b` vector)
#' @param j is the x coordinates of the rectangle (in the `a` vector)
#' @param i.len is the vertical length of the rectangle
#' @param j.len is the horizontal length of the rectangle
#' @param a is the first list
#' @param b is the second list
#' @param m is the number of P-values to compute between (i, .) and (i + i.len, .)
#' @param n is the number of P-values to compute between (., j) and (., j + j.len)
#' @param mode is the enrichment mode: "hyper", "hyper-two-tailed" or "hyper-two-tailed-old"
#' @param LOG is a flag if set return -log pval
#'
#' @useDynLib RedRibbon rrho_r_rectangle
#' @export
rrho_rectangle  <- function (i, j, i.len, j.len, m, n, a, b, mode=c("hyper"), LOG=FALSE)
{
    .Call("rrho_r_rectangle", as.integer(i), as.integer(j), as.integer(i.len), as.integer(j.len),
          as.integer(m), as.integer(n), as.double(a), as.double(b), as.character(mode), as.logical(LOG))
}


#' Call to C function (internal use): Search for the minimal P-value enrichment in a rectangle
#' 
#' @param i is the y coordinates of the rectangle (in the `b` vector)
#' @param j is the x coordinates of the rectangle (in the `a` vector)
#' @param i.len is the vertical length of the rectangle
#' @param j.len is the horizontal length of the rectangle
#' @param m is the number of P-values to compute between (i, .) and (i + i.len, .) (only used for `algorithm="classic"`)
#' @param n is the number of P-values to compute between (., j) and (., j + j.lenn)
#' @param a is the first list
#' @param b is the second list
#' @param mode is the enrichment mode: "hyper", "hyper-two-tailed" or "hyper-two-tailed-old"
#' @param direction is the enrichment mode if set to `enrichment` search for enrichment, otherwise search for underenrichment
#' 
#' @useDynLib RedRibbon rrho_r_rectangle_min
#' @export
rrho_rectangle_min  <- function (i, j, i.len, j.len, m, n, a, b, mode=c("hyper"), direction="enrichment")
{
    .Call("rrho_r_rectangle_min", as.integer(i), as.integer(j), as.integer(i.len), as.integer(j.len),
          as.integer(m), as.integer(n), as.double(a), as.double(b), as.character(mode), as.character(direction))
}


#' Call to C function (internal use): Search for the minimal P-value enrichment in a rectangle using evolutionnary algorithm
#' 
#' @param i is the y coordinates of the rectangle (in the `b` vector)
#' @param j is the x coordinates of the rectangle (in the `a` vector)
#' @param i.len is the vertical length of the rectangle
#' @param j.len is the horizontal length of the rectangle
#' @param a is the first list
#' @param b is the second list
#' @param mode is the enrichment mode: "hyper", "hyper-two-tailed" or "hyper-two-tailed-old"
#' @param direction is the enrichment mode if set to `enrichment` search for enrichment, otherwise search for underenrichment
#' 
#' @useDynLib RedRibbon rrho_r_rectangle_min_ea
#' @export
rrho_rectangle_min_ea  <- function (i, j, i.len, j.len, a, b, mode=c("hyper"), direction="enrichment")
{
    .Call("rrho_r_rectangle_min_ea", as.integer(i), as.integer(j), as.integer(i.len), as.integer(j.len),
             as.double(a), as.double(b), as.character(mode), as.character(direction))
}


## SEXP rrho_r_permutation(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP a, SEXP b, SEXP algo_params,
##		   SEXP mode, SEXP direction, SEXP algorithm,
##		   SEXP correlation,
##		   SEXP niter, SEXP pvalue_i, SEXP pvalue_j)

#' Call to C function (internal use): Permutation in a rectangle
#' 
#' @param i is the y coordinates of the rectangle (in the `b` vector)
#' @param j is the x coordinates of the rectangle (in the `a` vector)
#' @param i.len is the vertical length of the rectangle
#' @param j.len is the horizontal length of the rectangle
#' @param a is the first list
#' @param b is the second list
#' @param algo_params is a list if 2 elements named m and n representing the number of P-value to compute between in the rectangle (only used for `algorithm="classic"`)
#' @param mode is the enrichment mode: "hyper", "hyper-two-tailed" or "hyper-two-tailed-old"
#' @param direction is the enrichment mode if set to `enrichment` search for enrichment, otherwise search for underenrichment
#' @param algorithm is the algorithm used either `classic` grid method or `ea` evolutionary algorithm
#' @param correlation is the correlation between the genes
#' @param niter is the number of permutation
#' @param pvalue_i is the y coordinate of the best P-value
#' @param pvalue_j is the x coordinate of the best P-value
#'
#' @useDynLib RedRibbon rrho_r_permutation
#' @export
rrho_permutation <- function (i, j, i.len, j.len, a, b, algo_params=NULL, mode=c("hyper"), direction="enrichment", algorithm="classic", correlation=NULL,
                              niter=96, pvalue_i, pvalue_j)
{
    if ( is.null( algo_params ) )
        algo_params <- list()

    if (! mode %in% c("hyper", "hyper-two-tailed", "hyper-two-tailed-old") )
        stop("`mode` should be either hyper, hyper-two-tailed or hyper-two-tailed-old")

    if ( is.null(algo_params[["m"]]) )
        algo_params[["m"]] <- as.integer(i.len)
    else
        algo_params[["m"]] <- as.integer(algo_params[["m"]])
    if ( is.null(algo_params[["n"]]) )
        algo_params[["n"]] <- as.integer(j.len)
    else
        algo_params[["n"]] <- as.integer(algo_params[["n"]])

    if (! is.null(correlation) && ! class(correlation) %in% c("ld", "ld_fit", "fc") )
        stop("Correlation should be NULL or of 'ld', `ld_fit or 'fc'` class")
    
    .Call("rrho_r_permutation", as.integer(i), as.integer(j), as.integer(i.len), as.integer(j.len),
          as.double(a), as.double(b), algo_params, as.character(mode), as.character(direction),
          as.character(algorithm), correlation, as.integer(niter), as.integer(pvalue_i), as.integer(pvalue_j))
}

## SEXP rrho_r_rrho(SEXP i, SEXP j, SEXP a, SEXP b, SEXP mode)

#' Call to C function (internal use): RedRibbon rrho_ij
#' 
#' @param i is the y coordinates of the rectangle (in the `b` vector)
#' @param j is the x coordinates of the rectangle (in the `a` vector)
#' @param a is the first list
#' @param b is the second list
#' @param mode is the enrichment mode: "hyper", "hyper-two-tailed" or "hyper-two-tailed-old"
#' 
#' @useDynLib RedRibbon rrho_r_rrho
#' @export
rrho_ij  <- function (i, j, a, b, mode=c("hyper"))
{
    .Call("rrho_r_rrho", as.integer(i), as.integer(j), as.double(a), as.double(b), as.character(mode))
}

## SEXP rrho_r_intersect(SEXP i, SEXP j, SEXP a, SEXP b, SEXP directions)
#' Call to C function (internal use): Compute the overlap
#'
#' @param i is the y coordinates of the rectangle (in the `b` vector)
#' @param j is the x coordinates of the rectangle (in the `a` vector)
#' @param a is the first list
#' @param b is the second list
#' @param directions is the direction in both list (a, b): "upup", "downup", "updown" or "downdown"
#' 
#' @useDynLib RedRibbon rrho_r_intersect
#' @export
rrho_intersect  <- function (i, j, a, b, directions=c("downdown"))
{
    .Call("rrho_r_intersect", as.integer(i), as.integer(j), as.double(a), as.double(b), as.character(directions))
}

## SEXP rrho_r_expression_prediction(SEXP mat, SEXP nbr_tested)

#' Compute dependencies between genes
#'
#' See vignette for examples.
#' 
#' @param mat an expression matrix
#' @param nbr.tested limit the number of tested correlation
#' 
#' @return list(index=..., beta=...)
#' 
#' @useDynLib RedRibbon rrho_r_expression_prediction
#' @export
rrho_expression_prediction <- function (mat, nbr.tested = -1)
{
    .Call("rrho_r_expression_prediction", as.matrix(mat), as.integer(nbr.tested))
}

## SEXP rrho_r_ldfit_prediction(SEXP half, SEXP pval, SEXP position)
#' RedRibbon rrho_ldfit_prediction
#' 
#' @param half is the half distance for the fitted function
#' @param pval are the pvalues
#' @param position is the position vector
#' 
#' @examples
#' 
#' library(RedRibbon)
#' deps <- rrho_ldfit_prediction(600, runif(100), 1:100)
#'
#' @useDynLib RedRibbon rrho_r_ldfit_prediction
#' @export
rrho_ldfit_prediction <- function (half, pval, position)
{
    .Call("rrho_r_ldfit_prediction", as.double(half), as.double(pval), as.integer(position))
}

## SEXP rrho_r_normalize(SEXP mat, SEXP ref, SEXP mode)

#' Normalize TPMs
#'
#' @param mat is a matrix of expression
#' @param ref are the index of the reference
#' @param mode "poisson", "ls_mean", "ls_variance", "geometric_mean"
#' 
#' @useDynLib RedRibbon rrho_r_normalize
#' @export
rrho_normalize <- function (mat, ref, mode=c("poisson", "ls_mean", "ls_variance", "geometric_mean"))
{
    if (! mode[1] %in% c("poisson", "ls_mean", "ls_variance", "geometric_mean") )
        stop("`mode` should be either poisson, ls_mean, ls_variance or geometric_mean")


    .Call("rrho_r_normalize", as.matrix(mat), as.integer(ref), as.character(mode[1]))
}
