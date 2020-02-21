
import java.util.Random;

public class Bathroom {

    private static final int USE_TIME_DEFAULT = 3000;
  
    public Bathroom() {}

    public void use() {

        Random rand = new Random();

        try {
            Thread.sleep(rand.nextInt(USE_TIME_DEFAULT));
        } catch (InterruptedException ex) {
            System.err.println("Bathroom usage has been interrupted.");
        }
    }

    public void use(int time) {
        try {
            Thread.sleep(time);
        } catch (InterruptedException ex) {
            System.err.println("Bathroom usage has been interrupted.");
        }
    }
}