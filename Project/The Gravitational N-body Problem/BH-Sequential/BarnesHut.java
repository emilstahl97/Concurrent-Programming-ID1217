/* N-Body simulation utilizing sequential Barnes-Hut algorithm with O(n log n)
  
    Features: Prints initial coordinates of bodies, performs simulation, prints final coordinates of bodies to STDOUT 
    
    usage under UNIX:
            javac BarnesHut.java
            java BarnesHut <gnumBodies> <numSteps> <far> <printNum>
*/

import java.util.Random;

public class BarnesHut {

    private static int MAX_BODIES = 240;
    private static int MAX_STEPS = 300000;
    private static int MAX_MASS = 1000;

    public static void main(String[] args) {
 

        if(args.length != 4)
            System.out.println("Executing with default arguments:\n");

        int gnumBodies = (args.length > 0) && (Integer.parseInt(args[0]) < MAX_BODIES) ? Integer.parseInt(args[0]) : MAX_BODIES;
        int numSteps = (args.length > 1) && (Integer.parseInt(args[1]) < MAX_STEPS) ? Integer.parseInt(args[1]) : MAX_STEPS;
        int mass = (args.length > 3) && (Integer.parseInt(args[3]) < MAX_MASS) ? Integer.parseInt(args[3]) : MAX_MASS;
        int printNum = (args.length > 4) ? Integer.parseInt(args[4]) : 5;

        System.out.println("gnumBodies = " + gnumBodies + "\nnumSteps = " + numSteps + "\nmass = " + mass + "\n\nPrinting initial coordinates of bodies:\n");

        Point[] points = new Point[gnumBodies];
        Random rand = new Random();

        for (int i = 0; i < gnumBodies; i++) {
            points[i] = new Point((10 * (i % (int) Math.sqrt(gnumBodies))) + rand.nextDouble() * 7,
                    10 * (i / (int) Math.sqrt(gnumBodies)) + rand.nextDouble() * 7, 0, 0, 0, 0, mass);
        }

        double far = (args.length > 2) ? Integer.parseInt(args[2]) : points[gnumBodies - 1].posX + 7;

        //print five first points to track
        for (int i = 0; i < printNum; i++) {
            System.out.println("body " + i + " at " + points[i].posX);
        }

        System.out.println("\nSimulating....");

        long startTime = System.currentTimeMillis();

        for (int i = 0; i < numSteps; i++) {
            Quad q = new Quad(0, 0, far);
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

        long endTime = System.currentTimeMillis();

        System.out.println("\nPrinting final coordinates of bodies:\n");

        for (int i = 0; i < printNum; i++) {
            System.out.println("body " + i + " at " + points[i].posX);
        }

        System.out.println("\nTotal execution time: " + (endTime-startTime) + " ms\n");
    }
}