import java.util.Random;




class Fork implements Comparable<Fork> {
    private static int nextId = 0;
    private int id;

    public Fork() {
        this.id = nextId++;
    }

    public int compareTo(Fork rhs) {
        return new Integer(this.id).compareTo(new Integer(rhs.id));
    }
}





class Philosopher {
    private int eatCount;
    private long waitTime;
    private long startWait;
    private Random random;

    protected int position;
    protected Fork left;
    protected Fork right;

    public Philosopher(int position, Fork left, Fork right, int seed) {
        if (left == right) {
            throw new java.lang.IllegalArgumentException("Philosopher must have 2 different forks.");
        }

        this.eatCount = 0;
        this.waitTime = 0;
        this.startWait = System.currentTimeMillis();
        this.random = new Random(seed);

        this.position = position;
        this.left = left;
        this.right = right;
    }

    public Philosopher(int position, Fork left, Fork right) {
        this(position, left, right, 0);
        this.random = new Random();
    }

    public void startEating() {
        waitTime += System.currentTimeMillis() - startWait;

        System.out.println("[Philosopher " + position + "] is eating.");
        tryToSleep(random.nextInt(100));

        ++eatCount;
        System.out.println("[Philosopher " + position + "] finished eating.");
    }

    public void startThinking() {
        System.out.println("[Philosopher " + position + "] is thinking.");
        tryToSleep(random.nextInt(100));

        System.out.println("[Philosopher " + position + "] is hungry.");
        startWait = System.currentTimeMillis();
    }

    public int getPosition() {
        return position;
    }

    public long getWaitTime() {
        return waitTime;
    }

    public int getEatCount() {
        return eatCount;
    }

    private void tryToSleep(int mseconds) {
        try {
            Thread.sleep(mseconds);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}





public class RunnablePhilosopher extends Philosopher implements Runnable {
    private volatile boolean stopFlag;

    RunnablePhilosopher(int position, Fork left, Fork right, int seed) {
        super(position, left, right, seed);
        this.stopFlag = false;
    }

    RunnablePhilosopher(int position, Fork left, Fork right) {
        super(position, left, right);
        this.stopFlag = false;
    }

    public void run() {
        while (!stopFlag) {
            startThinking();
            Fork first = (left.compareTo(right) < 0) ? left : right;
            Fork second = (left.compareTo(right) < 0) ? right : left;

            synchronized (first) {
                    System.out.println("[Philosopher " + position
                            + "] took first fork.");
                synchronized (second) {
                    System.out.println("[Philosopher " + position
                            + "] took second fork.");
                    startEating();
                }
            }
        }

        System.out.println("[Philosopher " + position + "] stopped.");
    }

    public void stop() {
        stopFlag = true;
    }

    public static void main(String[] args)
    throws java.lang.InterruptedException {
        int count = 5;
        int seed = 360;

        RunnablePhilosopher[] phils = new RunnablePhilosopher[count];

        Fork last = new Fork();
        Fork left = last;
        for (int i = 0; i < count; ++i) {
            Fork right = (i == count - 1) ? last : new Fork();
            phils[i] = new RunnablePhilosopher(i, left, right, seed + i);
            left = right;
        }

        Thread[] threads = new Thread[count];

        for (int i = 0; i < count; ++i) {
            threads[i] = new Thread(phils[i]);
            threads[i].start();
        }

        Thread.sleep(60000);
        
        for (RunnablePhilosopher phil : phils) {
            phil.stop();
        }

        for (Thread thread : threads) {
            thread.join();
        }

        for (RunnablePhilosopher phil : phils) {
            System.out.println("Philosopher " + phil.getPosition() + " ate = "
                    + phil.getEatCount() + " times and waited = " + phil.getWaitTime() + " ms.");
        }
    }
}
