public class PercolationStats {
    private int N;
    private int T;
    private double[] arrayOfResults;

    public PercolationStats(int N, int T) {
        if (N <= 0 || T <= 0) {
            throw new java.lang.IllegalArgumentException(
                "PercolationStats(int N,int T): N and T should be greater than 0.");
        }

        this.N = N;
        this.T = T;
        this.arrayOfResults = new double[T];

        for (int experementNo = 0; experementNo < T; ++experementNo) {
            Percolation anPercolation = new Percolation(N);

            int numberOfOpenings = 0;
            while (!anPercolation.percolates()) {
                openRandomNonBlockedSite(anPercolation);
                ++numberOfOpenings;
            }

            arrayOfResults[experementNo] =
                    (double) numberOfOpenings / (N * N);
        }
    }

    public double mean() {
        double result = 0;
        for (double experementResult : arrayOfResults) {
            result += experementResult / T;
        }

        return result;
    }

    public double stddev() {
        double result = 0;
        double calculatedMean = this.mean();
        for (double experementResult : arrayOfResults) {
            result += (experementResult - calculatedMean)
                      * (experementResult - calculatedMean) / (T - 1);
        }

        return Math.sqrt(result);
    }

    public double confidenceLo() {
        return mean() - 1.96 * stddev() / Math.sqrt(T);
    }

    public double confidenceHi() {
        return mean() + 1.96 * stddev() / Math.sqrt(T);
    }

    public static void main(String[] args) {
        if (args.length < 2) {
            return;
        }

        PercolationStats summary = new PercolationStats(
                Integer.parseInt(args[0]),
                Integer.parseInt(args[1]));

        System.out.println(String.format(
                "mean                       = %f",
                summary.mean()));
        System.out.println(String.format(
                "stddev                     = %f",
                summary.stddev()));
        System.out.println(String.format(
                "95%% confidence interval    = %f, %f",
                summary.confidenceLo(),
                summary.confidenceHi()));

        // PercolationStats ps = new PercolationStats(0, 1);
    }

    private void openRandomNonBlockedSite(Percolation aPercolation) {
        int i = StdRandom.uniform(N) + 1;
        int j = StdRandom.uniform(N) + 1;

        while (aPercolation.isOpen(i, j)) {
            i = StdRandom.uniform(N) + 1;
            j = StdRandom.uniform(N) + 1;
        }

        aPercolation.open(i, j);
    }
}
