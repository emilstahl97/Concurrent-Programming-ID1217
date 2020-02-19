
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.Date;

public class BathroomMonitor {


    public BathroomState.State state = BathroomState.State.Empty;

    BathroomState bathroomState = new BathroomState();

    private final Lock lock = new ReentrantLock();
    private final Condition activeMen  = lock.newCondition(); 
    private final Condition activeWomen = lock.newCondition();
    private final Bathroom bathroom;

    private static final String WOMAN = "\uD83D\uDEBA";
    private static final String MAN = "\uD83D\uDEB9";

    /**
     * Sets the bathroom object.
     * @param bathroom The bathroom to let the entering people use.
     */
    public BathroomMonitor(Bathroom bathroom, int numMen, int numWomen) {
        if(bathroom == null) 
            throw new IllegalArgumentException("Can not set a bathroom which is null");
        this.bathroom = bathroom;
    }
    
    /**
     * This method will use the bathroom directly if no women are waiting or,
     * if there are women waiting, wait until they are done.
     */
    public void manEnter() {
        bathroomState.printQueues();
        System.out.println(MAN +" wants to enter");
        lock.lock();
        try {
            bathroomState.menInQueue++;
          while (bathroomState.womenInBathroom > 0) {
              try {
                  activeWomen.await();
              } catch (InterruptedException ex) {
                  System.err.println(MAN +" interrupted while waiting for bathroom");
              }
          }
          bathroomState.menInQueue--;
          state = BathroomState.State.MenEntering;
          bathroomState.menInBathroom++;
          bathroomState.printQueues();
          System.out.println(MAN +" enters");
          bathroom.use();
        } finally {
          lock.unlock();
        }
    }
    
    /**
     * Notifies that the man is done.
     * If it's the last man, notify all men.
     */
    public void manExit() {
        lock.lock();
        try {
            state = BathroomState.State.MenLeaving;
            bathroomState.menInBathroom--;
            bathroomState.printQueues();
            System.out.println(MAN +" leaves");
            if(bathroomState.menInBathroom == 0) 
                activeMen.signalAll();
        } finally {
          lock.unlock();
        }
    }
 
    /**
     * This method will use the bathroom directly if no men are waiting or,
     * if there are men waiting, wait until they are done.
     */
    public void womanEnter() {
        bathroomState.printQueues();
        System.out.println(WOMAN + " wants to enter");
       lock.lock();
        try {
            bathroomState.womenInQueue++;
          while (bathroomState.menInBathroom > 0) {
              try {
                  activeMen.await();
              } catch (InterruptedException ex) {
                  System.err.println(WOMAN +" interrupted while waiting for bathroom");
              }
          }
          bathroomState.womenInQueue--;
          state = BathroomState.State.WomenEntering;
          bathroomState.womenInBathroom++;
          
          bathroom.use();
          
          bathroomState.printQueues();
          System.out.println(WOMAN + " leaves");
        } finally {
          lock.unlock();
        }
    }
    
    /**
     * Notifies that the woman is done.
     * If it's the last woman, notify all men.
     */
    public void womanExit() {
        lock.lock();
        try {
            state = BathroomState.State.WomenLeaving;
            bathroomState.womenInBathroom--;
            bathroomState.printQueues();
            System.out.println(WOMAN +" leaves");
            if(bathroomState.womenInBathroom == 0) 
                activeWomen.signalAll();
        } finally {
          lock.unlock();
        }
    }
}

