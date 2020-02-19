
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class BathroomMonitor {

    public enum BathroomState {

        Empty,
        MenEntering,
        WomenEntering,
        MenLeaving,
        WomenLeaving,
    }

    private BathroomState state = BathroomState.Empty;

    private final Lock lock = new ReentrantLock();
    private final Condition activeMen  = lock.newCondition(); 
    private final Condition activeWomen = lock.newCondition();
    private final Bathroom bathroom;
    private int numMen;
    private int numWomen;
    private int time = 0;
    private int menInQueue = 0;
    private int menInBathroom = 0;
    private int womenInQueue = 0;
    private int womenInBathroom = 0;

    public static final String ANSI_RESET = "\u001B[0m";
    public static final String ANSI_PURPLE = "\u001B[35m";
    public static final String ANSI_BLUE = "\u001B[34m";
    public static final String ANSI_RED = "\u001B[31m";
    public static final String ANSI_GREEN = "\u001B[32m";
    public static final String ANSI_YELLOW = "\u001B[33m";

    public static final char BLACK_SQUARE = '\u25A0';
    public static final char WHITE_SQUARE = '\u25A1';

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
        printQueues();
        System.out.println("Man wants to enter");
        lock.lock();
        try {
            menInQueue++;
          while (womenInBathroom > 0) {
              try {
                  activeWomen.await();
              } catch (InterruptedException ex) {
                  System.err.println("Man interrupted while waiting for bathroom");
              }
          }
          menInQueue--;
          state = BathroomState.MenEntering;
          menInBathroom++;

          printQueues();
          System.out.println("Man enters");
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
            state = BathroomState.MenLeaving;
            menInBathroom--;
            printQueues();
            System.out.println("Man leaves");
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
        printQueues();
        System.out.println("Woman wants to enter");
       lock.lock();
        try {
            womenInQueue++;
          while (menInBathroom > 0) {
              try {
                  activeMen.await();
              } catch (InterruptedException ex) {
                  System.err.println("Woman interrupted while waiting for bathroom");
              }
          }
          womenInQueue--;
          state = BathroomState.WomenEntering;
          womenInBathroom++;
          
          bathroom.use();
          
          printQueues();
          System.out.println("Woman leaves");
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
            state = BathroomState.WomenLeaving;
            womenInBathroom--;
            printQueues();
            System.out.println("Woman leaves");
            if(womenInBathroom == 0) 
                activeWomen.signalAll();
        } finally {
          lock.unlock();
        }
    }

    private void printQueues() {
        String timeStamp = "";
        if (time < 10)
            timeStamp = "[ " + time++ + "  ] ";
        else if (time >= 10 && time < 100)
            timeStamp = "[ " + time++ + " ] ";
        else if (time >= 100 && time < 1000)
            timeStamp = "[" + time++ + " ] ";
        else
            timeStamp = "[" + time++ + "] ";

        String stateColour = "";
        String statePadded = "";
        switch (state) {
            case Empty:
                stateColour = ANSI_YELLOW;
                statePadded = "    Empty     ";
                break;
            case MenEntering:
                stateColour = ANSI_GREEN;
                statePadded = " Men Entering ";
                break;
            case MenLeaving:
                stateColour = ANSI_RED;
                statePadded = " Men Leaving  ";
                break;
            case WomenEntering:
                stateColour = ANSI_GREEN;
                statePadded = "Women Entering";
                break;
            case WomenLeaving:
                stateColour = ANSI_RED;
                statePadded = "Women Leaving ";
                break;
        }

        String stateString = stateColour + "State: [" + statePadded + "] " + ANSI_RESET;

        String menQueue = "[";
        for (int i = 0; i < menInQueue; i++)
            menQueue += BLACK_SQUARE;
        for (int i = 0; i < numMen - menInQueue; i++)
            menQueue += WHITE_SQUARE;
        menQueue += "]";

        String womenQueue = "[";
        for (int i = 0; i < womenInQueue; i++)
            womenQueue += BLACK_SQUARE;
        for (int i = 0; i < numWomen - womenInQueue; i++)
            womenQueue += WHITE_SQUARE;
        womenQueue += "]";

        String menBathroom = "[";
        for (int i = 0; i < menInBathroom; i++)
            menBathroom += BLACK_SQUARE;
        for (int i = 0; i < numMen - menInBathroom; i++)
            menBathroom += ' ';
        menBathroom += "]";

        String womenBathroom = "[";
        for (int i = 0; i < womenInBathroom; i++)
            womenBathroom += BLACK_SQUARE;
        for (int i = 0; i < numWomen - womenInBathroom; i++)
            womenBathroom += ' ';
        womenBathroom += "]";

        System.out.print(timeStamp + stateString + "Bathroom: " + ANSI_BLUE + "M:" + menBathroom + ANSI_PURPLE + "W:" + womenBathroom + ANSI_RESET + " Queues: " + ANSI_BLUE + "M:" + menQueue + ANSI_PURPLE + "W:" + womenQueue + ANSI_RESET + " ");

    }


}