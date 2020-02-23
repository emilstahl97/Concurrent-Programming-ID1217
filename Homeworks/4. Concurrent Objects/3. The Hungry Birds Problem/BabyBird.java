public class BabyBird implements Runnable {

    private Dish dish;
    public int id;

    public BabyBird(Dish Dish, int id) {

        this.dish = dish;
        this.id = id;
    }

    public void run() {

        while(true) {

            try {
                Thread.sleep(1000);
            } catch (InterruptedException exception) {}
        
        dish.eat(id);
        }
    }
}