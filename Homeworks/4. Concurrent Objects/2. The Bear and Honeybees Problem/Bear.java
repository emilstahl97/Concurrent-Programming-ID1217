public class Bear implements Runnable {

    HoneyPot honeyPot;

    public Bear(HoneyPot honeyPot) {

        this.honeyPot = honeyPot;
    }

    public void run() {

        while(true) {

            try {
                Thread.sleep(1000);
            } catch (InterruptedException exception) {}
        
        honeyPot.eat();
        }
    }
}