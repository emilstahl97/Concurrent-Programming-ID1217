import javax.print.DocFlavor.STRING;

public class HoneyPot {


    private static int FULL = 20;
    private static int EATEN = 0;
    public int honeyPot = 0;
    public int id;
    private static final String BEE = "\ud83d\udc1d";
    private static final String BEAR = "\ud83d\udc3b";
    private static final String HONEY = "\ud83c\udf6f";


    public synchronized void create(int id) {

        this.id = id;

        while(honeyPot == FULL) {

            try {
                wait();
            } catch(InterruptedException exception) {
                System.out.println("Honey creation interrupted");
            }
        }
        honeyPot++;
        System.out.println(BEE + " nr " + id + " created " + HONEY + " Quantity = " + honeyPot);
         
        if(honeyPot == FULL) {

            notifyAll();

            try {
                wait(); 
            } catch (InterruptedException exception) {}
        }
    }

    public synchronized void eat() {

        while(honeyPot != FULL) {

            try {
                wait();
            } catch (InterruptedException exception) {}
        }
        honeyPot = EATEN;
        System.out.println(BEAR + " ate all the  " + HONEY);

        notifyAll();
    }
}
