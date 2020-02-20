import java.util.Random;


public class Male implements Runnable {

    private Random rand = new Random();
    public int numVisits;
    private BathroomMonitor bathroommonitor;
    public int id; 
    public int visit = 1;

    public Male(BathroomMonitor bathroommonitor, int numVisits, int id) {

        this.bathroommonitor = bathroommonitor;
        this.numVisits = numVisits;
        this.id = id;    
    }
    
    public void run() {

        try {

            while(visit < numVisits + 1) {

                Thread.sleep(rand.nextInt(5000));
                bathroommonitor.manEnter(this);
                Thread.sleep(rand.nextInt(2000));
                //toilet.use();
                
               bathroommonitor.manExit(this); 

               
            }
        }
        catch (InterruptedException exception) {
            Thread.currentThread().interrupt();
            throw new RuntimeException(exception);
        }
    }
}