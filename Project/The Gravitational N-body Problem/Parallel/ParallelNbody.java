
import java.util.Random;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.logging.Level;
import java.util.logging.Logger;


 class ParallelNbody {

    public static int MAX_WORKERS = 20;
    public static int MAX_BODIES = 100;
    public static int MAX_STEPS = 300000;
    public static int gnumBodies;
    public static int numSteps;
    public static int numWorkers;
    public double DT = 1;  
    double G = 6.67e-11;
    double massOfBodies = 10;
    Point[] points;
   
    CyclicBarrier barrier;

    public ParallelNbody() {
        points = new Point[gnumBodies];
        final Random r = new Random();

        barrier = new CyclicBarrier(numWorkers);

        for (int i = 0; i < gnumBodies; i++) {
            points[i] = new Point((10 * (i % (int) Math.sqrt(gnumBodies))) + r.nextDouble() * 7,
                    10 * (i / (int) Math.sqrt(gnumBodies)) + r.nextDouble() * 7, 0, 0, 0, 0, massOfBodies, numWorkers);
        }
    }

    public void barrier(final int w) {
        try {
            barrier.await();
        } catch (final InterruptedException ex) {
        } catch (final BrokenBarrierException ex) {
            // Logger.getLogger(ParallelNbody.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public void calculateForces(final int w) {
        double distance, magnitude, dirX, dirY;

        for (int i = w; i < (int) gnumBodies; i += numWorkers) {
            for (int j = i + 1; j < gnumBodies; j++) {
                distance = distance(points[i], points[j]);
                magnitude = (G * points[i].mass * points[j].mass) / (distance * distance);
                dirX = points[j].posX - points[i].posX;
                dirY = points[j].posY - points[i].posY;
                points[i].forcesX[w] = points[i].forcesX[w] + magnitude * dirX / distance;
                points[j].forcesX[w] = points[j].forcesX[w] - magnitude * dirX / distance;
                points[i].forcesY[w] = points[i].forcesY[w] + magnitude * dirY / distance;
                points[j].forcesY[w] = points[j].forcesY[w] - magnitude * dirX / distance;
            }
        }
    }

    public static double distance(final Point a, final Point b) {
        return Math.sqrt(Math.pow((a.posX - b.posX), 2) + Math.pow((a.posY - b.posY), 2));
    }

    public void moveBodies(final int w) {
        double deltaVelX, deltaVelY, deltaPosX, deltaPosY, forceX = 0, forceY = 0;
        for (int i = w; i < gnumBodies; i += numWorkers) {
            for (int k = 0; k < numWorkers; k++) {
                forceX += points[i].forcesX[k];
                points[i].forcesX[k] = 0;

                forceY += points[i].forcesY[k];
                points[i].forcesY[k] = 0;
            }
            deltaVelX = (forceX / points[i].mass) * DT;
            deltaVelY = (forceY / points[i].mass) * DT;
            deltaPosX = (points[i].velX + deltaVelX / 2) * DT;
            deltaPosY = (points[i].velY + deltaVelY / 2) * DT;

            points[i].velX = points[i].velX + deltaVelX;
            points[i].velY = points[i].velY + deltaVelY;
            points[i].posX = points[i].posX + deltaPosX;
            points[i].posY = points[i].posY + deltaPosY;

            forceX = forceY = 0;
        }
    }

    public static void main(final String[] args) {

        if (args.length != 2)
            System.out.println("Executing with default arguments:\n");

        gnumBodies = (args.length > 0) && (Integer.parseInt(args[0]) < MAX_BODIES) ? Integer.parseInt(args[0])
                : MAX_BODIES;
        numSteps = (args.length > 1) && (Integer.parseInt(args[1]) < MAX_STEPS) ? Integer.parseInt(args[1]) : MAX_STEPS;
        numWorkers = (args.length > 2) && (Integer.parseInt(args[2]) < MAX_WORKERS) ? Integer.parseInt(args[2])
                : MAX_WORKERS;

        System.out.println("gnumBodies = " + gnumBodies + "\nnumSteps = " + numSteps + "\n numWorkers = " + numWorkers);

         ParallelNbody simulation = new ParallelNbody();

        for (int i = 0; i < numWorkers; i++) {
            final Worker worker = new Worker(i, simulation, numSteps);
            worker.start();
        }
    }
}