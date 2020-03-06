
import java.util.Random;
import java.util.concurrent.CyclicBarrier;


public class BarnesHut {

    public static int MAX_WORKERS = 20;
    public static int MAX_BODIES = 100;
    public static int MAX_STEPS = 300000;

    Point[] points;
    double G = 6.67e-11;
    double DT = 0.1;

    public static int numWorkers = 3;
    public static int gnumBodies = 120;
    public static int numSteps = 275000;
    double maxlength;
    double theta = 0.8;

    CyclicBarrier barrier;

    public BarnesHut() {

        int massOfBodies = 10;

        points = new Point[gnumBodies];

        barrier = new CyclicBarrier(numWorkers);

        //INITIALIZE POINTS
        Random r = new Random();
        for (int i = 0; i < gnumBodies; i++) {
            points[i] = new Point((10 * (i % (int) Math.sqrt(gnumBodies))) + r.nextDouble() * 7,
                    10 * (i / (int) Math.sqrt(gnumBodies)) + r.nextDouble() * 7, 0, 0, 0, 0, massOfBodies);
        }

        maxlength = points[gnumBodies - 1].posX + 7;
    }

    public static void main(String[] args) {

        if (args.length != 2)
            System.out.println("Executing with default arguments:\n");

        gnumBodies = (args.length > 0) && (Integer.parseInt(args[0]) < MAX_BODIES) ? Integer.parseInt(args[0]) : MAX_BODIES;
        numSteps = (args.length > 1) && (Integer.parseInt(args[1]) < MAX_STEPS) ? Integer.parseInt(args[1]) : MAX_STEPS;
        numWorkers = (args.length > 2) && (Integer.parseInt(args[2]) < MAX_WORKERS) ? Integer.parseInt(args[2]) : MAX_WORKERS;

        System.out.println("gnumBodies = " + gnumBodies + "\nnumSteps = " + numSteps + "\nnumWorkers = " + numWorkers);

        BarnesHut simulation = new BarnesHut();
        CyclicBarrier barrier = new CyclicBarrier(numWorkers);

        for (int i = 0; i < simulation.numWorkers; i++) {
            Worker worker = new Worker(i, simulation, barrier);
            worker.start();
        }

    }

}