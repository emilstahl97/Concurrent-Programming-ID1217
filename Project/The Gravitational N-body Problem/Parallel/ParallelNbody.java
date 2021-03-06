/* N-Body simulation utilizing parallel brute-force algorithm with O(n^2)
  
    Features: Prints initial coordinates of bodies, performs simulation, prints final coordinates of bodies to STDOUT 
    
    usage under UNIX:
            javac ParallelNbody.java
            java ParallelNbody <gnumBodies> <numSteps> <numWorkers <massOfBodies>
*/

import java.util.Random;
import java.util.concurrent.CyclicBarrier;

 class ParallelNbody {

    public static int MAX_WORKERS = 20;
    public static int MAX_BODIES = 240;
    public static int MAX_STEPS = 300000;
    public static int MAX_MASS = 10;
    public static int massOfBodies;
    public static int gnumBodies;
    public static int numSteps;
    public static int numWorkers;
    public double DT = 1;  
    double G = 6.67e-11;
    Point[] points;

    public ParallelNbody() {
        points = new Point[gnumBodies];
        final Random r = new Random();

        for (int i = 0; i < gnumBodies; i++) {
            points[i] = new Point((10 * (i % (int) Math.sqrt(gnumBodies))) + r.nextDouble() * 7,
                    10 * (i / (int) Math.sqrt(gnumBodies)) + r.nextDouble() * 7, 0, 0, 0, 0, massOfBodies, numWorkers);
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

        if (args.length != 4)
            System.out.println("Executing with default arguments:\n");

        gnumBodies = (args.length > 0) && (Integer.parseInt(args[0]) < MAX_BODIES) ? Integer.parseInt(args[0]) : MAX_BODIES;
        numSteps = (args.length > 1) && (Integer.parseInt(args[1]) < MAX_STEPS) ? Integer.parseInt(args[1]) : MAX_STEPS;
        numWorkers = (args.length > 2) && (Integer.parseInt(args[2]) < MAX_WORKERS) ? Integer.parseInt(args[2]) : MAX_WORKERS;
        massOfBodies = (args.length > 3) ? Integer.parseInt(args[3]) : MAX_MASS;

        System.out.println("\ngnumBodies = " + gnumBodies + "\nnumSteps = " + numSteps + "\nnumWorkers = " + numWorkers + "\nmass = " + massOfBodies + "\n\nPrinting initial coordinates of bodies:\n");

        ParallelNbody simulation = new ParallelNbody();
        CyclicBarrier barrier = new CyclicBarrier(numWorkers);

        for (int i = 0; i < numWorkers; i++) {
            final Worker worker = new Worker(i, numSteps, simulation, barrier);
            worker.start();
        }
    }
}