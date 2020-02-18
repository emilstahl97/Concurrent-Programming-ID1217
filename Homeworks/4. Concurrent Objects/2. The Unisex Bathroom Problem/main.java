

public class main {

    public static void main(String[] args) {

        int numMen, numWomen, numVisits;

        if(args.length < 3) {
        System.out.println("Usage: <numWomen> <numMen> <numVisits>");
        System.exit(1);
        }

        System.out.print("got here\n");   
        numWomen = Integer.parseInt(args[0]);
        numMen = Integer.parseInt(args[1]);
        numVisits = Integer.parseInt(args[2]);

        BathroomMonitor bathroommonitor = new BathroomMonitor(new Bathroom(), numWomen, numWomen);
        System.out.print("got here\n");   

        for(int i = 0; i < numWomen; i++) {
            Female female = new Female(bathroommonitor, numVisits, i);
            new Thread(female).start();
        }
        for(int i = 0; i < numMen; i++) {
            Male male = new Male(bathroommonitor, numVisits, i);
            new Thread(male).start();
        }


    }
}
