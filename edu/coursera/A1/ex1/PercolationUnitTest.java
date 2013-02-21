import junit.framework.Assert;
import junit.framework.JUnit4TestAdapter;

import org.junit.Test;
import org.junit.runner.JUnitCore;

public class PercolationUnitTest extends Assert {
    @Test
    public void testTrivialGrid() {
        Percolation p = new Percolation(1);
        assertTrue(!p.isOpen(1, 1));
        assertTrue(!p.isFull(1, 1));
        assertTrue(!p.percolates());

        System.out.println("CALL: OPEN()");
        p.open(1, 1);
        System.out.println("END CALL: OPEN()");

        assertTrue(p.isOpen(1, 1));
        assertTrue(p.isFull(1, 1));
        assertTrue(p.percolates());
    }

    @Test
    public void testSmallGrid() {
        Percolation p = new Percolation(4);

        p.open(1, 4);
        assertTrue(!p.percolates());

        p.open(2, 4);
        assertTrue(!p.percolates());

        p.open(2, 3);
        assertTrue(!p.percolates());

        p.open(2, 2);
        assertTrue(!p.percolates());

        p.open(3, 2);
        assertTrue(!p.percolates());

        p.open(3, 1);
        assertTrue(!p.percolates());

        p.open(4, 3);
        assertTrue(!p.percolates());

        p.open(4, 4);
        assertTrue(!p.percolates());

        p.open(1, 1);
        assertTrue(!p.percolates());

        p.open(4, 1);
        assertTrue(p.percolates());
    }

    @Test
    public void stressTest() {
        StdRandom.setSeed(360);

        for (int test = 0; test < 100; ++test) {
            System.out.println("stest " + (test + 1));
            int N = StdRandom.uniform(2, 100);
            boolean[][] aGrid = new boolean[N][N];

            Percolation foo = new Percolation(N);

            while (!foo.percolates()) {
                if (TrivialPercolation.percolates(aGrid)) {
                    System.out.println("Grid is percolate, but percolation is not!");
                    TrivialPercolation.printGrid(aGrid);

                    fail();
                }

                int i = StdRandom.uniform(N);
                int j = StdRandom.uniform(N);
                foo.open(i + 1, j + 1);
                aGrid[i][j] = true;
            }
            if (!TrivialPercolation.percolates(aGrid)) {
                System.out.println("Grid is not percolate, but percolation is!");
                TrivialPercolation.printGrid(aGrid);

                fail();
            }
        }
    }

    public static junit.framework.Test suite() {
        return new JUnit4TestAdapter(PercolationUnitTest.class);
    }

    public static void main(String[] args) {
        new JUnitCore().main("PercolationUnitTest");
    }
}

class TrivialPercolation {
    private static boolean[][] wasHere;

    public static boolean percolates(boolean[][] aGrid) {
        wasHere = new boolean[aGrid.length][aGrid.length];

        return tryToPercolate(aGrid, -2, 0);
    }

    public static void printGrid(boolean[][] aGrid) {
        for (int row = 0; row < aGrid.length; ++row) {
            for (boolean side : aGrid[row]) {
                if (side) {
                    System.out.print(". ");
                    continue;
                }
                System.out.print("# ");
            }

            System.out.println("");
        }
    }

    private static boolean tryToPercolate(boolean[][] aGrid, int i, int j) {
        if (i == -2) {
            boolean result = false;
            for (int x = 0; x < aGrid.length; ++x) {
                result = result || tryToPercolate(aGrid, 0, x);
                if (result) {
                    return true;
                }
            }

            return false;
        }

        if (i >= aGrid.length || j >= aGrid.length || i < 0 || j < 0) {
            return false;
        }

        if (wasHere[i][j]) {
            return false;
        }
        wasHere[i][j] = true;

        if (!aGrid[i][j]) {
            return false;
        }

        if (i + 1 == aGrid.length) {
            return true;
        }

        return tryToPercolate(aGrid, i + 1, j)
            || tryToPercolate(aGrid, i - 1, j)
            || tryToPercolate(aGrid, i, j + 1)
            || tryToPercolate(aGrid, i, j - 1);
    }
}
