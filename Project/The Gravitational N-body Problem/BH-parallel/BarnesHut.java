/* N-Body simulation utilizing parallel Barnes-Hut algorithm with O(n log n)
  
    Features: Prints initial coordinates of bodies, performs simulation, prints final coordinates of bodies to STDOUT 
    
    usage under UNIX:
            javac BarnesHut.java
            java BarnesHut <gnumBodies> <numSteps> <far> <numWorkers> <printNum>
*/

import java.util.Random;
import java.util.concurrent.CyclicBarrier;

public class BarnesHut {

    public static int MAX_WORKERS = 20;
    public static int MAX_BODIES = 240;
    public static int MAX_STEPS = 300000;
    public static int gnumBodies;
    public static int numSteps;
    public static int numWorkers;
    public static int printNum;
    public static double far;

    Point[] points;
    String[] args;

    CyclicBarrier barrier;

    public BarnesHut(String[] args) {

        this.args = args;
        int massOfBodies = 10;

        points = new Point[gnumBodies];

        //INITIALIZE POINTS
        Random r = new Random();
        for (int i = 0; i < gnumBodies; i++) {
            points[i] = new Point((10 * (i % (int) Math.sqrt(gnumBodies))) + r.nextDouble() * 7,
                    10 * (i / (int) Math.sqrt(gnumBodies)) + r.nextDouble() * 7, 0, 0, 0, 0, massOfBodies);
        }

        far = (args.length > 2) ? Integer.parseInt(args[2]) : points[gnumBodies - 1].posX + 7;
    }

    public static void main(String[] args) {

        if (args.length < 5)
            System.out.println("Executing with default arguments:\n");

        gnumBodies = (args.length > 0) && (Integer.parseInt(args[0]) < MAX_BODIES) ? Integer.parseInt(args[0]) : MAX_BODIES;
        numSteps = (args.length > 1) && (Integer.parseInt(args[1]) < MAX_STEPS) ? Integer.parseInt(args[1]) : MAX_STEPS;
        numWorkers = (args.length > 3) && (Integer.parseInt(args[3]) < MAX_WORKERS) ? Integer.parseInt(args[3]) : MAX_WORKERS;
        printNum = (args.length > 4) ? Integer.parseInt(args[4]) : 5;
        
        System.out.println("gnumBodies = " + gnumBodies + "\nnumSteps = " + numSteps + "\nnumWorkers = " + numWorkers + "\n");

        BarnesHut simulation = new BarnesHut(args);
        CyclicBarrier barrier = new CyclicBarrier(numWorkers);

        for (int i = 0; i < numWorkers; i++) {
            Worker worker = new Worker(i, numSteps, gnumBodies, numWorkers, far, simulation, barrier, printNum);
            worker.start();
        }
    }
}