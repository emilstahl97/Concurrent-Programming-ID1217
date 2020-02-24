public class HoneyPot {

    private int id;
    private int FULL;
    private int honeyPot = 0;
    private static int EATEN = 0;
    private static final String BEE = "\ud83d\udc1d";
    private static final String BEAR = "\ud83d\udc3b";
    private static final String HONEY = "\ud83c\udf6f";

    public HoneyPot(int maxHoney) {

        this.FULL = maxHoney;
    } 

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
         
        if(honeyPot == FULL) 
            notifyAll();
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
