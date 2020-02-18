
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * This class will act as a monitor for the access to the specified Bathroom.
 * It will use condition variables for synchronization in a Signal and Continue 
 * way.
 * @author Kim
 */
public class BathroomMonitor {

    private final Lock lock = new ReentrantLock();
    private final Condition activeMen  = lock.newCondition(); 
    private final Condition activeWomen = lock.newCondition();
    private final Bathroom bathroom;
    private int womenInBathroom = 0;
    private int menInBathroom = 0;
    private int numMen;
    private int numWomen;

    /**
     * Sets the bathroom object.
     * @param bathroom The bathroom to let the entering people use.
     */
    public BathroomMonitor(Bathroom bathroom, int numMen, int numWomen) {
        if(bathroom == null) 
            throw new IllegalArgumentException("Can not set a bathroom which is null");
        this.bathroom = bathroom;
        this.numMen = numMen;
        this.numWomen = numWomen;
    }
    
    /**
     * This method will use the bathroom directly if no women are waiting or,
     * if there are women waiting, wait until they are done.
     */
    public void manEnter() {
        System.out.println("got into manenter\n");
        lock.lock();
        try {
          while (womenInBathroom > 0) {
              try {
                  activeWomen.await();
              } catch (InterruptedException ex) {
                  System.err.println("Man interrupted while waiting for bathroom");
              }
          }
          
          menInBathroom++;
          System.out.println("before batroom.use");
          bathroom.use();
          System.out.println("after batroom.use");
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
            menInBathroom--;
            if(menInBathroom == 0) 
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
       lock.lock();
        try {
          while (menInBathroom > 0) {
              try {
                  activeMen.await();
              } catch (InterruptedException ex) {
                  System.err.println("Woman interrupted while waiting for bathroom");
              }
          }
          
          womenInBathroom++;
          bathroom.use();
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
            womenInBathroom--;
            if(womenInBathroom == 0) 
                activeWomen.signalAll();
        } finally {
          lock.unlock();
        }
    }
}