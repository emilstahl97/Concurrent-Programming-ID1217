public class Honeybees {

    private static int HONEY = 150;
    private static int MAX_BEES = 25;
    private static int EATEN = 0;

    public int honeyPot = 0;


    public static void main(String[] args) {

        int numBees;

        numBees = (args.length > 0) ? Integer.parseInt(args[0]) : MAX_BEES;

        System.out.println("Starting: \n numBees = " + numBees + "\n");

        HoneyPot honeyPot = new HoneyPot();

       for(int i = 0; i < numBees; i++) {
           Bee bee = new Bee(honeyPot, i);
           new Thread(bee).start();
       }

       Bear bear = new Bear(honeyPot);
       new Thread(bear).start();
     }

}