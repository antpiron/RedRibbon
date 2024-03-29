% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/rrho.R
\name{ggRedRibbon.rrho}
\alias{ggRedRibbon.rrho}
\title{Render the RRHO map.}
\usage{
\method{ggRedRibbon}{rrho}(
  self,
  n = NULL,
  labels = c("a", "b"),
  show.quadrants = TRUE,
  quadrants = NULL,
  show.pval = TRUE,
  repel.force = 150,
  base_size = 20,
  ...
)
}
\arguments{
\item{self}{is the RedRibbon object}

\item{n}{is the number of coordinates to compute on the x and y axis (Default = sqrt(len))}

\item{labels}{is a list of labels of list a and b. Default is c("a", "b").}

\item{show.quadrants}{is a flag if set the quadrants lines are drawn}

\item{quadrants}{is the object returned by 'quadrants()' method}

\item{show.pval}{is a flag to show the P-values on the plot}

\item{repel.force}{is the value of the repel force for the p-value ploting (default = 150)}

\item{base_size}{is the size of the text fields (default = 20)}

\item{...}{The rest of the parameters}
}
\value{
A \code{ggplot} object.
}
\description{
You can choose to render the RedRibbon level map using \code{ggplot2}.
}
