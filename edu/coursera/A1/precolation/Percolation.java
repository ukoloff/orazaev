public class Percolation {
    private int N;
    private boolean[] grid;

    private WeightedQuickUnionUF unionFind;
    private WeightedQuickUnionUF unionFindFillSides;
    private int TOP;
    private int BOTTOM;


    public Percolation(int N) {
        this.N = N;
        this.grid = new boolean[N * N];

        this.unionFind = new WeightedQuickUnionUF(N * N + 2);
        this.unionFindFillSides = new WeightedQuickUnionUF(N * N + 1);
        this.TOP = N * N;
        this.BOTTOM = N * N + 1;
    }

    public void open(int i, int j) {
        if (!isCorrectIndex(i) || !isCorrectIndex(j)) {
            throw new java.lang.IndexOutOfBoundsException();
        }

        grid[getIndex(i, j)] = true;

        connectWithNearPoints(i, j);
        connectWithTopAndBottom(i, j);
    }

    public boolean isOpen(int i, int j) {
        if (!isCorrectIndex(i) || !isCorrectIndex(j)) {
            throw new java.lang.IndexOutOfBoundsException();
        }

        return this.grid[getIndex(i, j)];
    }

    public boolean isFull(int i, int j) {
        if (!isCorrectIndex(i) || !isCorrectIndex(j)) {
            throw new java.lang.IndexOutOfBoundsException();
        }

        return unionFindFillSides.connected(getIndex(i, j), TOP);
    }

    public boolean percolates() {
        return unionFind.connected(TOP, BOTTOM);
    }


    public static void main(String[] args) {
    }


    private void connectOpenPointWithAnother(int i, int j, int i1, int j1) {
        if (!isCorrectIndex(i) || !isCorrectIndex(i1)
            || !isCorrectIndex(j) || !isCorrectIndex(j1)) {
            return;
        }

        if (isOpen(i1, j1)) {
            unionFind.union(getIndex(i, j), getIndex(i1, j1));
            unionFindFillSides.union(getIndex(i, j), getIndex(i1, j1));
        }
    }

    private void connectWithNearPoints(int i, int j) {
        connectOpenPointWithAnother(i, j, i + 1, j);
        connectOpenPointWithAnother(i, j, i - 1, j);
        connectOpenPointWithAnother(i, j, i, j + 1);
        connectOpenPointWithAnother(i, j, i, j - 1);
    }

    private boolean isOnTop(int i, int j) {
        return i == 1;
    }

    private boolean isOnBottom(int i, int j) {
        return i == this.N;
    }

    private void connectWithTopAndBottom(int i, int j) {
        if (isOnTop(i, j)) {
            unionFind.union(TOP, getIndex(i, j));
            unionFindFillSides.union(TOP, getIndex(i, j));
        }

        if (isOnBottom(i, j)) {
            unionFind.union(BOTTOM, getIndex(i, j));
        }
    }

    private int getIndex(int i, int j) {
        return (i - 1) * N + (j - 1);
    }

    private boolean isCorrectIndex(int i) {
        return i >= 1 && i <= N;
    }
}
