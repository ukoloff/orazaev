public class PercolationStats {
    public PercolationStats(int N, int T) {
        if (N <= 0 || T <= 0) {
            throw new java.lang.IllegalArgumentException(
                    "PercolationStats(int N,int T): N and T should be greater than 0.");
        }
    }

    public double mean() {
        return 0;
    }

    public double stddev() {
        return 0;
    }

    public double confidenceLo() {
        return 0;
    }

    public double confidenceHi() {
        return 0;
    }

    public static void main(String[] args) {
        // PercolationStats ps = new PercolationStats(0, 1);

        System.out.println(java.lang.Math.pow(10, 2));
    }
}
