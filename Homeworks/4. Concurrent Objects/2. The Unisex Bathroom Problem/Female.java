import java.util.Random;


public class Female implements Runnable {

    private Random rand = new Random();
    private BathroomMonitor bathroommonitor;
    public int numVisits;
    public int id; 
    public int visit = 1;

    public Female(BathroomMonitor bathroommonitor, int numVisits, int id) {

        this.bathroommonitor = bathroommonitor;
        this.numVisits = numVisits;
        this.id = id;    
    }

    public void run() {

        try {

            while(visit < numVisits + 1) {

                Thread.sleep(500);
                bathroommonitor.womanEnter(this);

               Thread.sleep(200);
               bathroommonitor.womanExit(this); 
            }
        }
        catch (InterruptedException exception) {
            Thread.currentThread().interrupt();
            throw new RuntimeException(exception);
        }
    }
}