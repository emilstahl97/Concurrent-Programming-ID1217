public class Dish {

    private int id;
    private int REFILL;
    private int worms;
    private static int EMPTY = 0;
    private static final String BABY_BIRD = "\ud83d\udc26";
    private static final String PARENT_BIRD = "\ud83e\udd85";
    private static final String WORM = "\ud83d\udc1b";

    public Dish(int maxWorms) {

        this.REFILL = maxWorms;
        this.worms = maxWorms;
    } 

    public synchronized void eat(int id) {

        this.id = id;

        while(worms == EMPTY) {

            try {
                wait();
            } catch(InterruptedException exception) {
                System.out.println("Eating interrupted");
            }
        }
        worms--;
        System.out.println(BABY_BIRD + " nr " + id + " ate a " + WORM + " Quantity = " + worms);
         
        if(worms == EMPTY) {

            System.out.println(BABY_BIRD + " nr " + id + " SQUEEEELS!!!!");
            notifyAll();
        }
    }

    public synchronized void refill() {

        while(worms != EMPTY) {

            try {
                wait();
            } catch (InterruptedException exception) {}
        }
        worms = REFILL;
        System.out.println(PARENT_BIRD + " added " + worms + " " + WORM + " to the dish");

        notifyAll();
    }
}
