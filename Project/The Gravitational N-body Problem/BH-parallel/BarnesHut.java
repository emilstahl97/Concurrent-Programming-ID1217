import java.util.concurrent.CyclicBarrier;
import java.util.Random;

class BarnesHut {

    public static int MAX_WORKERS = 20;
    public static int MAX_BODIES = 100;
    public static int MAX_STEPS = 300000;
    public static int gnumBodies;
    public static int numSteps;
    public static int numWorkers;
    public static double maxlength = 0;
    public double DT = 1;  
    double G = 6.67e-11;
    int massOfBodies = 10;
    Point[] points;
   

    public BarnesHut() {
        points = new Point[gnumBodies];
        final Random r = new Random();

        for (int i = 0; i < gnumBodies; i++) {
            points[i] = new Point((10 * (i % (int) Math.sqrt(gnumBodies))) + r.nextDouble() * 7,
                    10 * (i / (int) Math.sqrt(gnumBodies)) + r.nextDouble() * 7, 0, 0, 0, 0, massOfBodies);
        }
        double maxlength = points[gnumBodies - 1].posX + 7;
    }

    public static void main(final String[] args) {

        if (args.length != 2)
            System.out.println("Executing with default arguments:\n");

        gnumBodies = (args.length > 0) && (Integer.parseInt(args[0]) < MAX_BODIES) ? Integer.parseInt(args[0]) : MAX_BODIES;
        numSteps = (args.length > 1) && (Integer.parseInt(args[1]) < MAX_STEPS) ? Integer.parseInt(args[1]) : MAX_STEPS;
        numWorkers = (args.length > 2) && (Integer.parseInt(args[2]) < MAX_WORKERS) ? Integer.parseInt(args[2]) : MAX_WORKERS;

        System.out.println("gnumBodies = " + gnumBodies + "\nnumSteps = " + numSteps + "\n numWorkers = " + numWorkers);

         BarnesHut simulation = new BarnesHut();
         CyclicBarrier barrier = new CyclicBarrier(numWorkers);

        for (int i = 0; i < numWorkers; i++) {
            final Worker worker = new Worker(i, numSteps, maxlength, gnumBodies, numWorkers, simulation, barrier);
            worker.start();
        }
    }
}