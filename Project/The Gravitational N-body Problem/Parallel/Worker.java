import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Worker extends Thread {

    int id;
    int numSteps;
    ParallelNbody work;
    CyclicBarrier barrier;

    public Worker(final int w, final int numSteps, final ParallelNbody work, final CyclicBarrier barrier) {
        
        this.id = w;
        this.work = work;
        this.numSteps = numSteps;
        this.barrier = barrier;
    }

    public void barrier(final int w) {
        try {
            barrier.await();
        } catch (final InterruptedException ex) {
        } catch (final BrokenBarrierException ex) {
            Logger.getLogger(ParallelNbody.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void run() {

        long start = 0, time = 0;

        if (id == 0) {
            for (int i = 0; i < 5; i++) {
                System.out.println("body " + i + " at " + work.points[i].posX);
            }
            System.out.println("\nSimulating....\n");
            start = System.nanoTime();
        }

        for (int i = 0; i < numSteps; i++) {
            work.calculateForces(id);
            barrier(id);
            work.moveBodies(id);
            barrier(id);
        }

        if (id == 0) {
            time = System.nanoTime() - start;
            System.out.println("Printing final coordinates of bodies\n");
            for (int i = 0; i < 5; i++) {
                System.out.println("body " + i + " at " + work.points[i].posX);
            }
            System.out.println("\nTotal execution time: " + time * Math.pow(10, -9) + " seconds\n");
        }
    }
}