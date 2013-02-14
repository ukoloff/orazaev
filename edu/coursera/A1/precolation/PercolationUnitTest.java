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

        p.open(1, 1);
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

    public static junit.framework.Test suite() {
        return new JUnit4TestAdapter(PercolationUnitTest.class);
    }

    public static void main(String[] args) {
        new JUnitCore().main("PercolationUnitTest");
    }
}
