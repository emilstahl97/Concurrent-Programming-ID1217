import javax.print.DocFlavor.STRING;

public class HoneyPot {


    private int FULL;
    private static int EMPTY = 0;
    public int worms = 0;
    public int id;
    private static final String BEE = "\ud83d\udc1d";
    private static final String BEAR = "\ud83d\udc3b";
    private static final String HONEY = "\ud83c\udf6f";

    public HoneyPot(int maxWorms) {

        this.FULL = maxWorms;
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
        worms = 25;
        System.out.println("Parent added");

        notifyAll();
    }
}
