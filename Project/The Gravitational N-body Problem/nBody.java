 class nBody {

    public static int gnumBodies;
    public static int numSteps;
    public double DT = 1;  
    public static int MAX_BODIES = 100;
    public static int MAX_STEPS = 300000;
    
    
    public nBody() {

    }

    public static void main(String[] args) {

        if(args.length != 2)
            System.out.println("Executing with default arguments:\n");

        gnumBodies = (args.length > 0) && (Integer.parseInt(args[0]) < MAX_BODIES) ? Integer.parseInt(args[0]) : MAX_BODIES;
        numSteps = (args.length > 1) && (Integer.parseInt(args[1]) < MAX_STEPS) ? Integer.parseInt(args[1]) : MAX_STEPS;

        System.out.println("gnumBodies = " + gnumBodies + "\nnumSteps = " + numSteps);
    }

}