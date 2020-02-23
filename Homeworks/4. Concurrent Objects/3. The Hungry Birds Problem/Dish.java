
public class Dish {


    private int REFILL;
    private static int EMPTY = 0;
    public int worms = 0;
    public int id;
    private static final String BEE = "\ud83d\udc1d";
    private static final String BEAR = "\ud83d\udc3b";
    private static final String HONEY = "\ud83c\udf6f";

    public Dish(int maxWorms) {

        this.REFILL = maxWorms;
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
        System.out.println("Bee nr " + id + " ate a worm: Quantity = " + worms);
         
        if(worms == EMPTY) {

            System.out.println("Bird nr " + id + " SQUEEEELS!!!!");
            notifyAll();

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
        worms = REFILL;
        System.out.println("Parent added " + worms + " the dish");

        notifyAll();
    }
}
