public class Honeybees {

    private static int MAX_BEES = 25;

    public static void main(String[] args) {

        int numBees;

        numBees = (args.length > 0) ? Integer.parseInt(args[0]) : MAX_BEES;

        System.out.println("numBees = " + numBees + "\n");
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {}

        HoneyPot honeyPot = new HoneyPot();

       for(int i = 0; i < numBees; i++) {
           Bee bee = new Bee(honeyPot, i);
           new Thread(bee).start();
       }

       Bear bear = new Bear(honeyPot);
       new Thread(bear).start();
     }

}