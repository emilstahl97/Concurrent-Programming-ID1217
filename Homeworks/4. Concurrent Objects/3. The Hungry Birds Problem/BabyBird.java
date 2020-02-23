public class BabyBird implements Runnable {

    private final Dish dish;
    public int id;

    public BabyBird(Dish dish, int id) {
        
        this.dish = dish;
        this.id = id;
    }

    public void run() {

    while(true) {

        try {
            Thread.sleep(100);
        } catch (InterruptedException exception) {}

            dish.eat(id);
        }
    }
} 