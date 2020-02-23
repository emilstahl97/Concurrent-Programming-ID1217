import javax.print.DocFlavor.STRING;

public class HoneyPot {


    private int FULL;
    private static int EMPTY = 0;
    public int honeyPot = 0;
    public int id;
    private static final String BEE = "\ud83d\udc1d";
    private static final String BEAR = "\ud83d\udc3b";
    private static final String HONEY = "\ud83c\udf6f";

    public HoneyPot(int maxHoney) {

        this.FULL = maxHoney;
    } 


    public synchronized void eat(int id) {

        this.id = id;

        while(honeyPot == EMPTY) {

            try {
                wait();
            } catch(InterruptedException exception) {
                System.out.println("Honey creation interrupted");
            }
        }
        honeyPot--;
        System.out.println(BEE + " nr " + id + " ate " + HONEY + " Quantity = " + honeyPot);
         
        if(honeyPot == EMPTY) {

            notifyAll();

            try {
                wait(); 
            } catch (InterruptedException exception) {}
        }
    }

    public synchronized void refill() {

        while(honeyPot != EMPTY) {

            try {
                wait();
            } catch (InterruptedException exception) {}
        }
        honeyPot = 25;
        System.out.println("Parent added");

        notifyAll();
    }
}
