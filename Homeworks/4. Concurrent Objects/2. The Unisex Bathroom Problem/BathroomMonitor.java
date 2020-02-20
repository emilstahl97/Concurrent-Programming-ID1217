public class BathroomMonitor {

    private final Bathroom bathroom;
    private final BathroomState bathroomState;

    public BathroomMonitor(Bathroom bathroom, BathroomState bathroomState) {
        
        this.bathroom = bathroom;
        this.bathroomState = bathroomState;
    }
    
    /**
     * This method will use the bathroom directly if no women are waiting or,
     * if there are women waiting, wait until they are done.
     */
    public synchronized void manEnter(Male man) throws InterruptedException {
       
        bathroomState.printQueues();
        System.out.println(bathroomState.MAN + " " + man.id +" wants to enter");
        try {
            bathroomState.menInQueue++;
          while (bathroomState.womenInBathroom > 0) {
              try {
                wait();
              } catch (InterruptedException ex) {
                  System.err.println(bathroomState.MAN + " " + man.id +" interrupted while waiting for bathroom");
              }
          }
          bathroomState.menInQueue--;
          bathroomState.state = BathroomState.State.MenEntering;
          bathroomState.menInBathroom++;
          bathroomState.printQueues();
          System.out.println(bathroomState.MAN + " " + man.id + " enters");
          bathroom.use();
        } 
        finally {

        }
    }
    
    /**
     * Notifies that the man is done.
     * If it's the last man, notify all men.
     */
    public synchronized void manExit(Male man) {
        
        bathroomState.state = BathroomState.State.MenLeaving;
        bathroomState.menInBathroom--;
        bathroomState.printQueues();
        
        System.out.println(bathroomState.MAN +" leaves");
        
        if(bathroomState.menInBathroom == 0) 
            notifyAll();
        
    }
 
    /**
     * This method will use the bathroom directly if no men are waiting or,
     * if there are men waiting, wait until they are done.
     */
    public synchronized void womanEnter() {

        bathroomState.printQueues();
        System.out.println(bathroomState.WOMAN + " wants to enter");
        try {
            bathroomState.womenInQueue++;
          while (bathroomState.menInBathroom > 0) {
              try {
                wait();
              } catch (InterruptedException ex) {
                  System.err.println(bathroomState.WOMAN +" interrupted while waiting for bathroom");
              }
          }
          bathroomState.womenInQueue--;
          bathroomState.state = BathroomState.State.WomenEntering;
          bathroomState.womenInBathroom++;
          
          bathroom.use();
          
          bathroomState.printQueues();
          System.out.println(bathroomState.WOMAN + " leaves");
        } finally {
        }
    }
    
    /**
     * Notifies that the woman is done.
     * If it's the last woman, notify all men.
     */
    public synchronized void womanExit() {

        bathroomState.state = BathroomState.State.WomenLeaving;
        bathroomState.womenInBathroom--;
        bathroomState.printQueues();
        
        System.out.println(bathroomState.WOMAN +" leaves");
        
        if(bathroomState.womenInBathroom == 0) 
            notifyAll();
        
    }
}

