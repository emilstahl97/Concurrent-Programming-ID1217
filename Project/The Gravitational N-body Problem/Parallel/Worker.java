public class Worker extends Thread {

    int id;
    int numSteps;
    ParallelNbody work;

    public Worker(int w, ParallelNbody work, int numSteps) {
        this.id = w;
        this.work = work;
        this.numSteps = numSteps;

    }

    @Override
    public void run() {

        long start = 0, end = 0;

        if (id == 0) {
            for (int i = 0; i < 5; i++) {
                System.out.println("body " + i + " at " + work.points[i].posX);
            }
            start = System.nanoTime();
        }

        for (int i = 0; i < numSteps; i++) {
            work.calculateForces(id);
            work.barrier(id);
            work.moveBodies(id);
            work.barrier(id);
        }
        if (id == 0) {
            for (int i = 0; i < 5; i++) {
                System.out.println("body " + i + " at " + work.points[i].posX);
            }
            end = System.nanoTime() - start;
            System.out.println("total execution time: " + end * Math.pow(10, -9) + " seconds");
        }
    }
}

