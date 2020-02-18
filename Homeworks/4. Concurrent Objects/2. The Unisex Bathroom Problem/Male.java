import java.util.Random;


public class Male implements Runnable {

    private Random rand = new Random();
    private int numVisits;
    private BathroomMonitor bathroommonitor;
    private int id; 
    private int i;

    public Male(BathroomMonitor bathroommonitor, int numVisits, int id) {

        this.bathroommonitor = bathroommonitor;
        this.numVisits = numVisits;
        this.id = id;    
    }
    
    public void run() {
        System.out.print("got into male before run\n");   

        try {

            while(i < numVisits) {

                Thread.sleep(500);
                bathroommonitor.manEnter();

               Thread.sleep(200);
               bathroommonitor.manExit(); 

               
            }
        }
        catch (InterruptedException exception) {
            Thread.currentThread().interrupt();
            throw new RuntimeException(exception);
        }
    }
}