public class ParentBird implements Runnable {

    private Dish dish;

    public ParentBird(Dish dish) {
        
        this.dish = dish;
    }

    public void run() {

        while(true) {

            try {
                Thread.sleep(100);
            } catch (InterruptedException exception) {}

            dish.refill();
        }
    }
} 