public class Bee implements Runnable {

    public int id;
    private HoneyPot honeyPot;

    public Bee(HoneyPot honeyPot, int id) {
        
        this.honeyPot = honeyPot;
        this.id = id;
    }


    public void run() {

    while(true) {

        try {
            Thread.sleep(100);
        } catch (InterruptedException exception) {}


        honeyPot.create(id);

    }


    }
    
} 