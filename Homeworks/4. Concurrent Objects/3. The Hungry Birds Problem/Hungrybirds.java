public class Hungrybirds {

    private static int MAX_BIRDS = 25;
    private static int MAX_WORMS = 25;

    public static void main(String[] args) {

        int numBirds, maxWorms;

        numBirds = (args.length > 0) && (Integer.parseInt(args[0]) < MAX_BIRDS) ? Integer.parseInt(args[0]) : MAX_BIRDS;
        maxWorms = (args.length > 1) && (Integer.parseInt(args[1]) < MAX_WORMS) ? Integer.parseInt(args[1]) : MAX_WORMS;

        System.out.println("Number of birds = " + numBirds + "\n" + "Number of worms = " + maxWorms + "\n");

        Dish dish = new Dish(maxWorms);

       for(int i = 0; i < numBirds; i++) {
           BabyBird babyBird = new BabyBird(dish, i);
           new Thread(babyBird).start();
       }

       ParentBird parentBird = new ParentBird(dish);
       new Thread(parentBird).start();
     }

}