
import java.util.Random;

public class Toilet {

    private static final int USE_TIME_DEFAULT = 2000;
    
    /**
     * Sets the simulated bathroom use time maxium to default (USE_TIME_DEFAULT).
     */
    public Toilet() {
    }

    public void use() {

        Random rand = new Random();
        
        try {
            Thread.sleep(USE_TIME_DEFAULT);
        } catch (InterruptedException ex) {
            System.err.println("Bathroom usage has been interrupted.");
        }
    }

}