public class Honeybees {

    private static int MAX_BIRDS = 25;
    private static int MAX_WORMS = 25;

    public static void main(String[] args) {

        int numBirds, maxWorms;

        numBirds = (args.length > 0) && (Integer.parseInt(args[0]) < MAX_BIRDS) ? Integer.parseInt(args[0]) : MAX_BIRDS;
        maxWorms = (args.length > 1) && (Integer.parseInt(args[1]) < MAX_WORMS) ? Integer.parseInt(args[1]) : MAX_WORMS;

        System.out.println("numBirds = " + numBirds + "\n" + "Max worms = " + maxWorms);
        System.out.println("args.length = " + args.length);

        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {}

        HoneyPot honeyPot = new HoneyPot(maxWorms);

       for(int i = 0; i < numBirds; i++) {
           Bee bee = new Bee(honeyPot, i);
           new Thread(bee).start();
       }

       Bear bear = new Bear(honeyPot);
       new Thread(bear).start();
     }

}