
class Worker extends Thread {

        int id;
        BarnesHut work;

        public Worker(int w, BarnesHut work) {
            this.id = w;
            this.work = work;
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

            for (int i = 0; i < work.numSteps; i++) {
                Quad q = new Quad(0, 0, work.maxlength);
                BHTree tree = new BHTree(q);

                //create tree
                for (int j = 0; j < work.gnumBodies; j++) {
                    if (work.points[j].in(q)) {
                        tree.insert(work.points[j]);
                    }
                }

                //calculate forces for assigned points
                for (int j = id; j < work.gnumBodies; j += work.numWorkers) {
                    tree.updateForce(work.points[j]);
                }

                //wait for other workers to finish
                work.barrier(id);

                //move the points according to the forces
                for (int j = id; j < work.gnumBodies; j += work.numWorkers) {
                    work.points[j].movePoint();
                }
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
