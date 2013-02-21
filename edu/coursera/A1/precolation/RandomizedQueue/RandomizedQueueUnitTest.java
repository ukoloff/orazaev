import junit.framework.Assert;
import junit.framework.JUnit4TestAdapter;

import org.junit.Test;
import org.junit.Before;
import org.junit.runner.JUnitCore;

public class RandomizedQueueUnitTest extends Assert {
    @Before
    public void runBeforeEveryTest() {
        StdRandom.setSeed(360);
    }

    @Test
    public void testTrivial() {
        RandomizedQueue<Integer> rq = new RandomizedQueue<Integer>();
        assertEquals(0, rq.size());
        assertTrue(rq.isEmpty());

        rq.enqueue(777);
        assertEquals(1, rq.size());
        assertTrue(!rq.isEmpty());

        assertEquals(777, (int) rq.sample());
        assertEquals(1, rq.size());

        assertEquals(777, (int) rq.dequeue());
        assertEquals(0, rq.size());
        assertTrue(rq.isEmpty());
    }

    @Test
    public void testMoreElements() {
        RandomizedQueue<Integer> rq = new RandomizedQueue<Integer>();

        boolean[] checkArray = new boolean[100];
        for (int i = 0; i < 100; ++i) {
            rq.enqueue(i);
        }
        
        assertEquals(100, rq.size());
        assertTrue(!rq.isEmpty());
        
        for (int i = 0; i < 100; ++i) {
            assertEquals(100 - i, rq.size());

            int next = rq.dequeue();
            assertTrue(!checkArray[next]);
            checkArray[next] = true;
        }

        assertEquals(0, rq.size());
        assertTrue(rq.isEmpty());
    }

    @Test
    public void testIteration() {
        RandomizedQueue<Integer> rq = new RandomizedQueue<Integer>();

        boolean[] checkArray = new boolean[100];
        for (int i = 0; i < 100; ++i) {
            rq.enqueue(i);
        }


        int[] firstSequence = new int[100];
        int[] secondSequence = new int[100];
        int i = 0;
        for (int item : rq) {
            assertTrue(!checkArray[item]);
            checkArray[item] = true;

            firstSequence[i++] = item;
        }

        assertEquals(100, rq.size());

        i = 0;
        for (int item : rq) {
            secondSequence[i++] = item;
        }

        boolean isSeqsEqual = true;
        for (i = 0; i < 100; ++i) {
            isSeqsEqual &= secondSequence[i] == firstSequence[i];
        }

        assertTrue(!isSeqsEqual);
        assertEquals(100, rq.size());
    }

    public static void main(String[] args) {
        new JUnitCore().main("RandomizedQueueUnitTest");
    }

    public static junit.framework.Test suite() {
        return new JUnit4TestAdapter(RandomizedQueueUnitTest.class);
    }
}
