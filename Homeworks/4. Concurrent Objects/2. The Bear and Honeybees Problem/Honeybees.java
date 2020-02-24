public class Honeybees {

    private static int MAX_BEES = 25;
    private static int MAX_HONEY = 25;

    public static void main(String[] args) {

        int numBees, maxHoney;

        numBees = (args.length > 0) && (Integer.parseInt(args[0]) < MAX_BEES) ? Integer.parseInt(args[0]) : MAX_BEES;
        maxHoney = (args.length > 1) && (Integer.parseInt(args[1]) < MAX_HONEY) ? Integer.parseInt(args[1]) : MAX_HONEY;
    
        System.out.println("Number of bees = " + numBees + "\n" + "Max honey = " + maxHoney + "\n");

        try {
            Thread.sleep(800);
        } catch (InterruptedException e) {}

        HoneyPot honeyPot = new HoneyPot(maxHoney);

       for(int i = 0; i < numBees; i++) {
           Bee bee = new Bee(honeyPot, i);
           new Thread(bee).start();
        }

       Bear bear = new Bear(honeyPot);
       new Thread(bear).start();
     }
}