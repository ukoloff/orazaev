public class Percolation {
    public Percolation(int N) {
        this.N = N;
        this.greed = new boolean[java.lang.Math.pow(N, 2)];

        // id 0 is top and id 1 is bottom
        this.unionFind = new QuickFindUF(java.lang.Math.pow(N, 2) + 2);
    }

    public void open(int i, int j) {
        this.greed[(i - 1) * N + (j - 1)] = true;

        connectWithNearPoints(i, j);
        connectWithTopAndBottom(i, j);
    }

    public boolean isOpen(int i, int j) {
        if (i < 1 || i > this.N || j < 1 || j > N) {
            return false;
        }

        return this.greed[(i - 1) * N + (j - 1)];
    }

    public boolean isFull(int i, int j) {
        return unionFind.connected((i - 1) * N + (j - 1) + 2, 0);
    }

    public boolean precolates() {
        return unionFind.connected(0, 1);
    }


    public static void main(String[] args) {
        Percolation foo = new Percolation(10);
    }


    private boolean[] grid;
    private int N;
    private QuickFindUF unionFind;


    private void connectOpenPointWithAnother(int i, int j, int i1, int j1) {
        if (this.isOpen(i1, j1)) {
            unionFind.union(
                    (i - 1) * N + (j - 1) + 2,
                    (i1 - 1) * N + (j1 - 1) + 2);
        }
    }

    private void connectWithNearPoints(int i, int j) {
        connectOpenPointWithAnother(i, j, i + 1, j);
        connectOpenPointWithAnother(i, j, i - 1, j);
        connectOpenPointWithAnother(i, j, i, j + 1);
        connectOpenPointWithAnother(i, j, i, j - 1);
    }

    private boolean isOnTop(int i, int j) {
        if (i == 1) {
            return true;
        }

        return false;
    }

    private boolean isOnBottom(int i, int j) {
        if (i == this.N) {
            return true;
        }

        return false;
    }

    private void connectWithTopAndBottom(int i, int j) {
        if (isOnTop(i, j)) {
            unionFind.union(
                0,
                (i - 1) * N + (j - 1) + 2);
        }

        if (isOnBottom(i, j)) {
            unionFind.union(
                1,
                (i - 1) * N + (j - 1) + 2);
        }
    }
}
