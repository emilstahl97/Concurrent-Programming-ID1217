

public class main {

    public static void main(String[] args) {

        int numMen, numWomen, numVisits;

        if(args.length < 3) {
        System.out.println("Usage: <numWomen> <numMen> <numVisits>");
        System.exit(1);
        }

        numWomen = Integer.parseInt(args[0]);
        numMen = Integer.parseInt(args[1]);
        numVisits = Integer.parseInt(args[2]);

        System.out.println("numWomen = " + numWomen + "\nnumMen = " + numMen + "\nnumVisits = "+numVisits+"\n");

        BathroomState bathroomState = new BathroomState();
        BathroomMonitor bathroommonitor = new BathroomMonitor(new Bathroom(), bathroomState);
        Work work = new Work();

        bathroomState.setNum(numWomen, numMen);
        
        for(int i = 0; i < numWomen; i++) {
            Female female = new Female(bathroommonitor, work, numVisits, i);
            new Thread(female).start();
        }
        for(int i = 0; i < numMen; i++) {
            Male male = new Male(bathroommonitor, work, numVisits, i);
            new Thread(male).start();
        }
    }
}