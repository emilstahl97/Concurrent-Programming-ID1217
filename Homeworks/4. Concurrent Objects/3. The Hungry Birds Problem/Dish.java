public class Dish {


    private int FULL;
    private static int EMPTY = 0;
    public int worms = 10;
    public int id;
    private static final String BEE = "\ud83d\udc1d";
    private static final String BEAR = "\ud83d\udc3b";
    private static final String HONEY = "\ud83c\udf6f";

    public Dish(int maxWorms) {

        this.FULL = maxWorms;
    } 


    public synchronized void eat(int id) {

        this.id = id;

        while(true) {

            if(worms > 0) {
                worms--;
                System.out.println("Bird nr "+  id + " ate a worm");
            }
            else {
                System.out.println("Bird nr " + id + " SQUEELS!!!!!");
                notifyAll();
            }
            try {
                wait();
            } catch (InterruptedException exception) {}
        }

        }

    public synchronized void refill() {

        while(worms != EMPTY) {

            try {
                wait();
            } catch (InterruptedException exception) {}
        }
        worms = 25;
        System.out.println("Parent bird refilled dish");

        notifyAll();
    }
}
