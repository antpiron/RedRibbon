## SEXP rrho_r_rectangle(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP m, SEXP n, SEXP a, SEXP b, SEXP mode, SEXP log_flag)
#' @useDynLib RedRibbon rrho_r_rectangle
#' @export
rrho_rectangle  <- function (i, j, i.len, j.len, m, n, a, b, mode=c("hyper"), LOG=FALSE)
{
    .Call("rrho_r_rectangle", as.integer(i), as.integer(j), as.integer(i.len), as.integer(j.len),
          as.integer(m), as.integer(n), as.double(a), as.double(b), as.character(mode), as.logical(LOG))
}


#' @useDynLib RedRibbon rrho_r_rectangle_min
#' @export
rrho_rectangle_min  <- function (i, j, i.len, j.len, m, n, a, b, mode=c("hyper"), direction="enrichment")
{
    .Call("rrho_r_rectangle_min", as.integer(i), as.integer(j), as.integer(i.len), as.integer(j.len),
          as.integer(m), as.integer(n), as.double(a), as.double(b), as.character(mode), as.character(direction))
}


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

#' RedRibbon Permutation
#' 
#' TODO: Description
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

#' RedRibbon rrho_ij
#' 
#' 
#' @useDynLib RedRibbon rrho_r_rrho
#' @export
rrho_ij  <- function (i, j, a, b, mode=c("hyper"))
{
    .Call("rrho_r_rrho", as.integer(i), as.integer(j), as.double(a), as.double(b), as.character(mode))
}

## SEXP rrho_r_intersect(SEXP i, SEXP j, SEXP a, SEXP b, SEXP directions)

#' RedRibbon rrho_intersect
#' 
#' 
#' @useDynLib RedRibbon rrho_r_intersect
#' @export
rrho_intersect  <- function (i, j, a, b, directions=c("downdown"))
{
    .Call("rrho_r_intersect", as.integer(i), as.integer(j), as.double(a), as.double(b), as.character(directions))
}

## SEXP rrho_r_expression_prediction(SEXP mat, SEXP nbr_tested)

#' RedRibbon rrho_r_expression_prediction
#' 
#' @param mat an expression matrix
#' 
#' @return correlation between gene to pass to RedRibbon(..., correlation=...) as correlation paramter
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
#' 
#' @useDynLib RedRibbon rrho_r_ldfit_prediction
#' @export
rrho_ldfit_prediction <- function (half, pval, position)
{
    .Call("rrho_r_ldfit_prediction", as.double(half), as.double(pval), as.integer(position))
}

## SEXP rrho_r_normalize(SEXP mat, SEXP ref, SEXP mode)

#' RedRibbon rrho_normalize
#' 
#' 
#' @useDynLib RedRibbon rrho_r_normalize
#' @export
rrho_normalize <- function (mat, ref, mode="poisson")
{
    .Call("rrho_r_normalize", as.matrix(mat), as.integer(ref), as.character(mode))
}
