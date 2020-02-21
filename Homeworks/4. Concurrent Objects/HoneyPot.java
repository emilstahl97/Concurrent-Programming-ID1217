public class HoneyPot {


    private static int FULL = 20;
    private static int EATEN = 0;
    public int honeyPot = 0;
    public int id;



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
        System.out.println("Bee nr " + id + " added some honey to the pot");
        
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
        System.out.println("The bear ate the honey");

        notifyAll();
    }
}
