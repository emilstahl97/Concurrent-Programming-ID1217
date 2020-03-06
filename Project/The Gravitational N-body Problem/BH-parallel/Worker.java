import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Worker extends Thread {

    int id;
    int numSteps;
    double maxLength;
    int gnumBodies;
    int numWorkers;
    BarnesHut work;
    CyclicBarrier barrier;

    public Worker(int w, int numSteps, double maxLength, int gnumBodies, int numWorkers, BarnesHut work, CyclicBarrier barrier) {
        
        this.id = w;
        this.numSteps = numSteps;
        this.maxLength = maxLength;
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
            Quad quad = new Quad(0, 0, maxLength);
            BHTree tree = new BHTree(quad);

            //create tree
            for (int j = 0; j < gnumBodies; j++) {
                if (work.points[j].in(quad)) {
                    tree.insert(work.points[j]);
                }
            }

            //calculate forces for assigned points
            for (int j = id; j < gnumBodies; j += numWorkers) {
                tree.updateForce(work.points[j]);
            }

            //wait for other workers to finish
            barrier(id);

            //move the points according to the forces
            for (int j = id; j < gnumBodies; j += numWorkers) {
                work.points[j].movePoint();
            }
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

