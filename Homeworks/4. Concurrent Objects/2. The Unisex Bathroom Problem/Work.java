public class Work {

    private static final int WORK_TIME = 500;


    public Work() {
        //this(WORK_TIME);
    }


    public void doWork() {

        try {
            Thread.sleep(WORK_TIME);
        }
        catch (InterruptedException exception) {
            System.out.println("Work");
        }
    }
} 