public class Bear implements Runnable {

    Dish dish;

    public Bear(Dish dish) {

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