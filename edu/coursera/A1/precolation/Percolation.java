public class Percolation {
    private int N;
    private boolean[] grid;
    private WeightedQuickUnionUF unionFind;


    public Percolation(int N) {
        this.N = N;
        this.grid = new boolean[N * N];

        // id 0 is top and id 1 is bottom
        this.unionFind = new WeightedQuickUnionUF(N * N + 2);
    }

    public void open(int i, int j) {
        this.grid[getIndex(i, j)] = true;

        connectWithNearPoints(i, j);
        connectWithTopAndBottom(i, j);
    }

    public boolean isOpen(int i, int j) {
        if (i < 1 || i > this.N || j < 1 || j > N) {
            return false;
        }

        return this.grid[getIndex(i, j)];
    }

    public boolean isFull(int i, int j) {
        return unionFind.connected(getIndex(i, j) + 2, 0);
    }

    public boolean percolates() {
        return unionFind.connected(0, 1);
    }


    public static void main(String[] args) {
    }


    private void connectOpenPointWithAnother(int i, int j, int i1, int j1) {
        if (this.isOpen(i1, j1)) {
            unionFind.union(getIndex(i, j) + 2, getIndex(i1, j1) + 2);
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
            unionFind.union(0, getIndex(i, j) + 2);
        }

        if (isOnBottom(i, j)) {
            unionFind.union(1, getIndex(i, j) + 2);
        }
    }

    private int getIndex(int i, int j) {
        return (i - 1) * N + (j - 1);
    }
}
