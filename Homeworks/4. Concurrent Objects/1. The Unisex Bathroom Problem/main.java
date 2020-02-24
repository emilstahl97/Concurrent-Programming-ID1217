public class Main {

private static int MAX_WOMEN = 10;
private static int MAX_MEN = 10;
private static int MAX_VISITS = 5;
    public static void main(String[] args) {

        int numMen, numWomen, numVisits;

        if(args.length < 3) {
            System.out.println("Usage: <numWomen> <numMen> <numVisits>");
            System.exit(1);
        }

        numWomen = (Integer.parseInt(args[0]) < MAX_WOMEN) ? Integer.parseInt(args[0]) : MAX_WOMEN;
        numMen = (Integer.parseInt(args[1]) < MAX_MEN) ? Integer.parseInt(args[1]) : MAX_MEN;
        numVisits = (Integer.parseInt(args[2]) < MAX_VISITS) ? Integer.parseInt(args[2]) : MAX_VISITS;

        System.out.println("numWomen = " + numWomen + "\nnumMen = " + numMen + "\nnumVisits = "+numVisits+"\n");

        Work work = new Work();
        Bathroom bathroom = new Bathroom();
        BathroomState bathroomState = new BathroomState();
        BathroomMonitor bathroommonitor = new BathroomMonitor(bathroom, bathroomState);

        bathroomState.setNum(numWomen, numMen);
        
        for(int i = 0; i < numWomen; i++) {
            Female female = new Female(bathroommonitor, work, bathroom, numVisits, i);
            new Thread(female).start();
        }
        for(int i = 0; i < numMen; i++) {
            Male male = new Male(bathroommonitor, work, bathroom, numVisits, i);
            new Thread(male).start();
        }
    }
}