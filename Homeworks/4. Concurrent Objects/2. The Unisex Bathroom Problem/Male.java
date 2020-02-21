import java.util.Random;


public class Male implements Runnable {

    private Random rand = new Random();
    public int numVisits;
    private BathroomMonitor bathroommonitor;
    private Work work;
    public int id; 
    public int visit = 1;

    public Male(BathroomMonitor bathroommonitor, Work work, int numVisits, int id) {

        this.bathroommonitor = bathroommonitor;
        this.work = work;
        this.numVisits = numVisits;
        this.id = id;    
    }
    
    public void run() {

        try {

            while(visit < numVisits + 1) {

                work.doWork();
                bathroommonitor.manEnter(this);

               Thread.sleep(200);
               bathroommonitor.manExit(this); 

               
            }
        }
        catch (InterruptedException exception) {
            Thread.currentThread().interrupt();
            throw new RuntimeException(exception);
        }
    }
}