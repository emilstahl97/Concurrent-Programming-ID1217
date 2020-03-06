
import java.util.Random;
import java.util.concurrent.CyclicBarrier;

public class BarnesHut {

    public static int MAX_WORKERS = 20;
    public static int MAX_BODIES = 100;
    public static int MAX_STEPS = 300000;
    public static int gnumBodies;
    public static int numSteps;
    public static int numWorkers;
    public static double maxlength;

    Point[] points;

    CyclicBarrier barrier;

    public BarnesHut() {

        int massOfBodies = 10;

        points = new Point[gnumBodies];

        //INITIALIZE POINTS
        Random r = new Random();
        for (int i = 0; i < gnumBodies; i++) {
            points[i] = new Point((10 * (i % (int) Math.sqrt(gnumBodies))) + r.nextDouble() * 7,
                    10 * (i / (int) Math.sqrt(gnumBodies)) + r.nextDouble() * 7, 0, 0, 0, 0, massOfBodies);
        }

        maxlength = points[gnumBodies - 1].posX + 7;
    }

    public static void main(String[] args) {

        if (args.length < 3)
            System.out.println("Executing with default arguments:\n");

        gnumBodies = (args.length > 0) && (Integer.parseInt(args[0]) < MAX_BODIES) ? Integer.parseInt(args[0]) : MAX_BODIES;
        numSteps = (args.length > 1) && (Integer.parseInt(args[1]) < MAX_STEPS) ? Integer.parseInt(args[1]) : MAX_STEPS;
        numWorkers = (args.length > 2) && (Integer.parseInt(args[2]) < MAX_WORKERS) ? Integer.parseInt(args[2]) : MAX_WORKERS;

        System.out.println("gnumBodies = " + gnumBodies + "\nnumSteps = " + numSteps + "\nnumWorkers = " + numWorkers + "\n");

        BarnesHut simulation = new BarnesHut();
        CyclicBarrier barrier = new CyclicBarrier(numWorkers);

        for (int i = 0; i < numWorkers; i++) {
            Worker worker = new Worker(i, numSteps, gnumBodies, numWorkers, maxlength, simulation, barrier);
            worker.start();
        }
    }
}