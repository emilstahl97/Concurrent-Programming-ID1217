import java.util.concurrent.ThreadLocalRandom;

public class vehicle implements Runnable {

    private int requestedNitrogen = 0;
    private int requestedQuantumFluid = 0; 

    public SpaceStation station;

    public vehicle(int name, SpaceStation myStation) {

        this.name = name; 
        this.station = myStation;

    }

    public void run() {

        ThreadLocalRandom random = new ThreadLocalRandom.current();

        for(int i = 0; i < 10; i++)
            try {
                Thread.sleep(rand.nextInt(5,20));
            }
            catch (InterruptedException ex) {}

            requestedNitrogen = rand.nextInt(1,10);
            requestedQuantumFluid = rand.nextInt(1,10);


    }
}