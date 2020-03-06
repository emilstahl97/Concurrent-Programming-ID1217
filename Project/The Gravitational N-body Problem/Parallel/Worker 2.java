import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.logging.Level;
import java.util.logging.Logger;


public class Worker extends Thread {

    int id;
    int numSteps;
    int gnumBodies;
    int numWorkers;
    BarnesHut work;
    CyclicBarrier barrier;

    public Worker(final int w, final int numSteps, int gnumBodies, int numWorkers, BarnesHut work, CyclicBarrier barrier) {
        
        this.id = w;
        this.numSteps = numSteps;
        this.gnumBodies = gnumBodies;
        this.numWorkers = numWorkers;
        this.work = work;
        this.barrier = barrier;
    }

    public void barrier(final int w) {
        try {
            barrier.await();
        } catch (final InterruptedException ex) {
        } catch (final BrokenBarrierException ex) {
            Logger.getLogger(BarnesHut.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void run() {

        long start = 0, end = 0;

        if (id == 0) {
            for (int i = 0; i < 5; i++) {
                System.out.println("body " + i + " at " + work.points[i].posX);
            }
            start = System.nanoTime();
        }

        for (int i = 0; i < numSteps; i++) {
            work.calculateForces(id);
            barrier(id);
            work.moveBodies(id);
            barrier(id);
        }
        if (id == 0) {
            for (int i = 0; i < 5; i++) {
                System.out.println("body " + i + " at " + work.points[i].posX);
            }
            end = System.nanoTime() - start;
            System.out.println("total execution time: " + end * Math.pow(10, -9) + " seconds");
        }
    }
}

