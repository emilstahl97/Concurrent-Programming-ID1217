public class ParentBird implements Runnable {

    private final Dish dish;

    public ParentBird(Dish dish) {

        this.dish = dish;
    }

    public void run() {

        while(true) {

            try {
                Thread.sleep(1000);
            } catch (InterruptedException exception) {}
        
            dish.refill();
        }
    }
}