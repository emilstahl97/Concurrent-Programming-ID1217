import java.util.Random;


public class Female implements Runnable {

    private Random rand = new Random();
    private BathroomMonitor bathroommonitor;
    private Work work;
    private Bathroom bathroom;
    public int numVisits;
    public int id; 
    public int visit = 1;

    public Female(BathroomMonitor bathroommonitor, Work work, Bathroom bathroom, int numVisits, int id) {

        this.bathroommonitor = bathroommonitor;
        this.work = work;
        this.bathroom = bathroom;
        this.numVisits = numVisits;
        this.id = id;    
    }

    public void run() {

        try {

            while(visit < numVisits + 1) {

                work.doWork();
                bathroommonitor.womanEnter(this);

               bathroommonitor.womanExit(this); 
            }
        }
        catch (InterruptedException exception) {
            Thread.currentThread().interrupt();
            throw new RuntimeException(exception);
        }
    }
}