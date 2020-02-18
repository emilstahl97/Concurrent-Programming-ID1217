

public class main {

    public static void main(String[] args) {

        int numMen, numWomen, numVisits;

        if(args.length < 3) {
        System.out.println("Usage: <numWomen> <numMen> <numVisits>");
        System.exit(1);
        }

        numWomen = Integer.parseInt(args[1]);
        numMen = Integer.parseInt(args[2]);
        numVisits = Integer.parseInt(args[3]);
    }
}
