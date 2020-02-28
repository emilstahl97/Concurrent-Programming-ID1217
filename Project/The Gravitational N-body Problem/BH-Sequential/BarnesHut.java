import java.util.Random;

public class BarnesHut {

    public static void main(String[] args) {

        Point[] points;
        double G = 6.67e-11;
        double DT = 0.1;

        int gnumBodies = 10;
        int numSteps = 275;

        double massOfBodies = 10;

        points = new Point[gnumBodies];

        //INITIALIZE POINTS
        Random r = new Random();
        for (int i = 0; i < gnumBodies; i++) {
            points[i] = new Point((10 * (i % (int) Math.sqrt(gnumBodies))) + r.nextDouble() * 7,
                    10 * (i / (int) Math.sqrt(gnumBodies)) + r.nextDouble() * 7, 0, 0, 0, 0, massOfBodies);
        }

        double maxlength = points[gnumBodies - 1].posX + 7;

        //print five first points to track
        for (int i = 0; i < 5; i++) {
            System.out.println("body " + i + " at " + points[i].posX);
        }

        long start = System.nanoTime();

        for (int i = 0; i < numSteps; i++) {
            Quad q = new Quad(0, 0, maxlength);
            BHTree tree = new BHTree(q);

            for (int j = 0; j < gnumBodies; j++) {
                if (points[j].in(q)) {
                    tree.insert(points[j]);
                }
            }
            for (int j = 0; j < gnumBodies; j++) {
                points[j].forceX = points[j].forceY = 0;
                tree.updateForce(points[j]);
                points[j].movePoint();
            }
        }

        long end = System.nanoTime() - start;

        for (int i = 0; i < 5; i++) {
            System.out.println("body " + i + " at " + points[i].posX);
        }

        System.out.println("Total execution time: " + end * Math.pow(10, -9) + " seconds");

    }
}