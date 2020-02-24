public class Male implements Runnable {

    public int visit = 1;
    public int id; 
    private Work work;
    public int numVisits;
    private Bathroom bathroom;
    private BathroomMonitor bathroommonitor;
    
    public Male(BathroomMonitor bathroommonitor, Work work, Bathroom bathroom, int numVisits, int id) {

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
                bathroommonitor.manEnter(this);
                bathroom.use(200);
                bathroommonitor.manExit(this); 
                Thread.sleep(10);               
            }
        } catch (InterruptedException exception) {
            Thread.currentThread().interrupt();
            throw new RuntimeException(exception);
        }
    }
}