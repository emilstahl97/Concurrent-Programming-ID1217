public class Honeybees {

    private static int MAX_BEES = 25;
    private static int MAX_HONEY = 25;

    public static void main(String[] args) {

        int numBees, maxHoney;

        numBees = (args.length > 0) ? Integer.parseInt(args[0]) : MAX_BEES;
        numBees = (Integer.parseInt(args[0]) > MAX_BEES) ? MAX_BEES : numBees;

        maxHoney = (args.length > 1) ? Integer.parseInt(args[1]) : MAX_HONEY;
        maxHoney = (maxHoney > MAX_HONEY) ? MAX_HONEY : maxHoney;

        System.out.println("numBees = " + numBees + "\n" + "Max honey = " + maxHoney);
        System.out.println("args.length = " + args.length);

        try {
            Thread.sleep(1000);
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