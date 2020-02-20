
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.Date;

public class BathroomMonitor {

    private final Lock lock = new ReentrantLock();
    private final Condition activeMen  = lock.newCondition(); 
    private final Condition activeWomen = lock.newCondition();
    private final Bathroom bathroom;
    private final BathroomState bathroomState;

    private static final String WOMAN = "\uD83D\uDEBA";
    private static final String MAN = "\uD83D\uDEB9";

    /**
     * Sets the bathroom object.
     * @param bathroom The bathroom to let the entering people use.
     */
    public BathroomMonitor(Bathroom bathroom, BathroomState bathroomState) {
        if(bathroom == null) 
            throw new IllegalArgumentException("Can not set a bathroom which is null");
        this.bathroom = bathroom;
        this.bathroomState = bathroomState;
    }
    
    /**
     * This method will use the bathroom directly if no women are waiting or,
     * if there are women waiting, wait until they are done.
     */
    public synchronized void manEnter() throws InterruptedException {
        bathroomState.printQueues();
        System.out.println(MAN +" wants to enter");
        try {
            bathroomState.menInQueue++;
          while (bathroomState.womenInBathroom > 0) {
              try {
                wait();
              } catch (InterruptedException ex) {
                  System.err.println(MAN +" interrupted while waiting for bathroom");
              }
          }
          bathroomState.menInQueue--;
          bathroomState.state = BathroomState.State.MenEntering;
          bathroomState.menInBathroom++;
          bathroomState.printQueues();
          System.out.println(MAN +" enters");
          bathroom.use();
        } 
        finally {

        }
    }
    
    /**
     * Notifies that the man is done.
     * If it's the last man, notify all men.
     */
    public synchronized void manExit() {
        
        bathroomState.state = BathroomState.State.MenLeaving;
        bathroomState.menInBathroom--;
        bathroomState.printQueues();
        
        System.out.println(MAN +" leaves");
        
        if(bathroomState.menInBathroom == 0) 
            notifyAll();
        
    }
 
    /**
     * This method will use the bathroom directly if no men are waiting or,
     * if there are men waiting, wait until they are done.
     */
    public synchronized void womanEnter() {

        bathroomState.printQueues();
        System.out.println(WOMAN + " wants to enter");
        try {
            bathroomState.womenInQueue++;
          while (bathroomState.menInBathroom > 0) {
              try {
                wait();
              } catch (InterruptedException ex) {
                  System.err.println(WOMAN +" interrupted while waiting for bathroom");
              }
          }
          bathroomState.womenInQueue--;
          bathroomState.state = BathroomState.State.WomenEntering;
          bathroomState.womenInBathroom++;
          
          bathroom.use();
          
          bathroomState.printQueues();
          System.out.println(WOMAN + " leaves");
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
        
        System.out.println(WOMAN +" leaves");
        
        if(bathroomState.womenInBathroom == 0) 
            notifyAll();
        
    }
}

