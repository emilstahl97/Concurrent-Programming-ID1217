public class Bee implements Runnable {

    public int id;
    private Dish dish;

    public Bee(Dish dish, int id) {
        
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