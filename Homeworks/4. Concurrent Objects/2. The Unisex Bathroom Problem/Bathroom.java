
import java.util.Random;

/**
 * This class represents a bathroom which you can use.
 * @author Kim
 */
public class Bathroom {

    private static final int USE_TIME_DEFAULT = 3 * 1000;
    private final int USE_TIME;
    
    /**
     * Sets the simulated bathroom use time maxium to default (USE_TIME_DEFAULT).
     */
    public Bathroom() {
        this(USE_TIME_DEFAULT);
    }
    
    /**
     * Sets the simulated use time to the specified parameter.
     * @param useTime The maximum time which a thread will use the bathroom. 
     */
    public Bathroom(int useTime) {
        if(useTime < 0) throw new IllegalArgumentException("Can not set a "
                + "negative bathroom use time.");
        USE_TIME = useTime;
    }

    /**
     * Simulates a bathroom usage by letting the specified thread sleep for 
     * a random amount of time.
     */
    public void use() {
        //System.out.println("inside bathroom.use\n");
        Random rand = new Random();
        int time = rand.nextInt(USE_TIME);

        try {
            System.out.println("inside use, thread sleeping\n");
            Thread.sleep(time);
        } catch (InterruptedException ex) {
            System.err.println("Bathroom usage has been interrupted.");
        }
    }

}