LDADD = ../src/libale.la
AM_LDFLAGS = -static
AM_CFLAGS = $(OPENMP_CFLAGS) -g -O0 -fsanitize=bounds -I$(top_srcdir)/src

dist_data_DATA = asntoowner.txt ipv4toasn.txt rir-ripencc.txt plop.gz
## ls test/t-*.c  | sed 's/.*\/\(t-.*\).c/\1/' | tr '\n' ' '
check_PROGRAMS = t-alg-ax-b-ols-solve t-alg-ax-b-ols-solve-full t-alg-ax-b-solve t-alg-l-inverse t-alg-lx-b-solve \
	t-alg-opposite-v t-alg-transpose t-alg-qr-qtb-householder t-alg-u-inverse t-alg-ux-b-solve \
	t-bitset \
	t-btree-init t-btree-insert t-btree-mknode t-btree-search t-btree-slice t-btree-split-internal-child \
	t-btree-split-leaf-child t-btree-split-right-internal t-btree-split-right-leaf t-btree-split-root \
	t-classifier-sl t-ea t-error-ret t-exparser t-fileutils-gzfopen t-fileutils-isgzip \
	t-hash t-index t-interpolation-linear t-knn \
	t-math-beta t-math-cmp-double t-math-digamma t-math-gamma t-math-ibeta t-math-lbeta t-math-lgamma t-math-rilgamma \
	t-math-riugamma t-math-trigamma \
	t-matrix-filter-rows t-matrix-init-destroy t-matrix-read \
	t-memory \
	t-mkpath t-mkpath-fopen t-netutils \
	t-optimisation-gradient-descent t-portability-getrandom t-portability-strcasestr t-process-popen \
	t-rmpath t-skiplist \
	t-sl-init t-sl-insert-remove-first-next-nth t-sl-mknode t-sl-search t-sl-stack-operations t-sort-q-indirect \
	t-stats-beta-F t-stats-beta-F-inv t-stats-beta-fit-mle t-stats-beta-fit-mle-newton t-stats-beta-fit-mm \
	t-stats-beta-rand t-stats-binom-f t-stats-binom-F t-stats-categorical-rand t-stats-cov t-stats-diff-mean \
	t-stats-diff-sd t-stats-diff-var t-stats-ecdf-F t-stats-ecdf-F-inv t-stats-ecdf-F-linear t-stats-ecdf-rand \
	t-stats-gamma-F t-stats-gamma-fit-mm t-stats-gamma-rand t-stats-hyper-f t-stats-hyper-F t-stats-hyper-tail \
	t-stats-kolmo-F t-stats-ks-test t-stats-mat-cov t-stats-mean t-stats-geom-mean \
	t-stats-normalize-samples-ls t-stats-normalize-samples-poisson t-stats-normalize-samples-geometric-mean t-stats-normalize-samples \
	t-stats-norm-F t-stats-norm-rand t-stats-norm-std-F t-stats-norm-std-rand \
	t-stats-F-F t-stats-p-adjust-fdr-bh t-stats-p-adjust-fwer-bonf t-stats-pearson-corr t-stats-permutation-bootstrap \
	t-stats-permutation t-stats-poisson-f t-stats-poisson-F t-stats-poisson-rand-its t-stats-poisson-rand-pa \
	t-stats-rowmeans t-stats-colmeans t-stats-sd t-stats-shuffle t-stats-student-F t-stats-student-rand \
	t-stats-t-test t-stats-t-test-paired t-stats-t-test-welch t-stats-unif-F t-stats-unif-rand t-stats-unif-std-F \
	t-stats-unif-std-rand t-stats-var t-strjoin t-strtolower t-unlink-path t-vector t-vla-string t-vla-string-split

TESTS = $(check_PROGRAMS)

check-valgrind:	$(check_PROGRAMS)
	@for prog in $(check_PROGRAMS); do echo "valgrind $$prog"; valgrind -q ./$$prog; done
