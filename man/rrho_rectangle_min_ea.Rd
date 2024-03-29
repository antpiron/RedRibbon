% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/calls.R
\name{rrho_rectangle_min_ea}
\alias{rrho_rectangle_min_ea}
\title{Call to C function (internal use): Search for the minimal P-value enrichment in a rectangle using evolutionnary algorithm}
\usage{
rrho_rectangle_min_ea(
  i,
  j,
  i.len,
  j.len,
  a,
  b,
  mode = c("hyper"),
  direction = "enrichment"
)
}
\arguments{
\item{i}{is the y coordinates of the rectangle (in the `b` vector)}

\item{j}{is the x coordinates of the rectangle (in the `a` vector)}

\item{i.len}{is the vertical length of the rectangle}

\item{j.len}{is the horizontal length of the rectangle}

\item{a}{is the first list}

\item{b}{is the second list}

\item{mode}{is the enrichment mode: "hyper", "hyper-two-tailed" or "hyper-two-tailed-old"}

\item{direction}{is the enrichment mode if set to `enrichment` search for enrichment, otherwise search for underenrichment}
}
\value{
The minimal p-value and coordinate in a rectangle with the evolutionary algorithm
}
\description{
Call to C function (internal use): Search for the minimal P-value enrichment in a rectangle using evolutionnary algorithm
}
