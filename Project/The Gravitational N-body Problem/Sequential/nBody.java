import java.util.Random;
class nBody {

    public static int gnumBodies;
    public static int numSteps;
    public double DT = 1;  
    double G = 6.67e-11;
    double massOfBodies = 10;
    public static int MAX_BODIES = 240;
    public static int MAX_STEPS = 300000;
    Point[] points;
   
    
    public nBody() {
        points = new Point[gnumBodies];
        Random r = new Random();

        for (int i = 0; i < gnumBodies; i++) {
            points[i] = new Point((10 * (i % (int) Math.sqrt(gnumBodies))) + r.nextDouble() * 7,
                    10 * (i / (int) Math.sqrt(gnumBodies)) + r.nextDouble() * 7, 0, 0, 0, 0, massOfBodies);
        }
    }

    public void calculateForces() {
        double distance, magnitude, directionX, directionY;

        for (int i = 0; i < gnumBodies; i++) {
            for (int j = i + 1; j < gnumBodies; j++) {
                distance = distance(points[i], points[j]);
                magnitude = (G * points[i].mass * points[j].mass) / (distance * distance);
                directionX = points[j].posX - points[i].posX;
                directionY = points[j].posY - points[i].posY;
                points[i].forceX = points[i].forceX + magnitude * directionX / distance;
                points[j].forceX = points[j].forceX - magnitude * directionX / distance;
                points[i].forceY = points[i].forceY + magnitude * directionY / distance;
                points[j].forceY = points[j].forceY - magnitude * directionY / distance;
            }
        }
    }

    public void moveBodies() {
        double deltaVelX;
        double deltaVelY;
        double deltaPosX;
        double deltaPosY;

        for (int i = 0; i < gnumBodies; i++) {
            deltaVelX = (points[i].forceX / points[i].mass) * DT;
            deltaVelY = (points[i].forceY / points[i].mass) * DT;

            deltaPosX = ((points[i].velX + deltaVelX) / 2) * DT;
            deltaPosY = ((points[i].velY + deltaVelY) / 2) * DT;

            points[i].velX = points[i].velX + deltaVelX;
            points[i].velY = points[i].velY + deltaVelY;

            points[i].posX = points[i].posX + deltaPosX;
            points[i].posY = points[i].posY + deltaPosY;

            points[i].forceX = 0;
            points[i].forceY = 0;
        }
    }

    public static double distance(Point a, Point b) {
        return Math.sqrt(Math.pow((a.posX - b.posX), 2) + Math.pow((a.posY - b.posY), 2));
    }



    public static void main(String[] args) {

        if(args.length != 2)
            System.out.println("Executing with default arguments:\n");

        gnumBodies = (args.length > 0) && (Integer.parseInt(args[0]) < MAX_BODIES) ? Integer.parseInt(args[0]) : MAX_BODIES;
        numSteps = (args.length > 1) && (Integer.parseInt(args[1]) < MAX_STEPS) ? Integer.parseInt(args[1]) : MAX_STEPS;

        System.out.println("gnumBodies = " + gnumBodies + "\nnumSteps = " + numSteps);

        nBody simulation = new nBody();

        for (int i = 0; i < 5; i++) {
            System.out.println("Body number " + i + " at position (" + simulation.points[i].posX + ", " + simulation.points[i].posY + ")");
        }
        
        long start = System.nanoTime();

        for (int i = 0; i < numSteps; i++) {
            simulation.calculateForces();
            simulation.moveBodies();
        }

        long time = System.nanoTime() - start;
        
        for (int i = 0; i < 5; i++) {
            System.out.println("Body number " + i + " at position (" + simulation.points[i].posX + ", " + simulation.points[i].posY + ")");
        }
        System.out.println("TIME: " + time*Math.pow(10, -9) + " seconds");

    }

}