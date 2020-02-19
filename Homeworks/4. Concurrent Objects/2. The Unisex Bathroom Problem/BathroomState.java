
public class BathroomState {

    public enum State {

        Empty,
        MenEntering,
        WomenEntering,
        MenLeaving,
        WomenLeaving,
    }

   public BathroomState.State state = BathroomState.State.Empty;

    public int menInQueue = 0;
    public int menInBathroom = 0;
    public int womenInQueue = 0;
    public int womenInBathroom = 0;
    public int numWomen;
    public int numMen;
    private int time;

    public static final String ANSI_RESET = "\u001B[0m";
    public static final String ANSI_PURPLE = "\u001B[35m";
    public static final String ANSI_BLUE = "\u001B[34m";
    public static final String ANSI_RED = "\u001B[31m";
    public static final String ANSI_GREEN = "\u001B[32m";
    public static final String ANSI_YELLOW = "\u001B[33m";

    public static final char BLACK_SQUARE = '\u25A0';
    public static final char WHITE_SQUARE = '\u25A1';

    public void setNum(int numWomen, int numMen) {

        this.numWomen = numWomen;
        this.numMen = numMen;
    }


    public void printQueues() {
        System.out.println("numMen = " + numMen);
        String timeStamp = "";
        if (time < 10)
            timeStamp = "[ " + time++ + "  ] ";
        else if (time >= 10 && time < 100)
            timeStamp = "[ " + time++ + " ] ";
        else if (time >= 100 && time < 1000)
            timeStamp = "[" + time++ + " ] ";
        else
            timeStamp = "[" + time++ + "] ";

        String stateColour = "";
        String statePadded = "";

        switch (state) {
            case Empty:
                stateColour = ANSI_YELLOW;
                statePadded = "    Empty     ";
                break;
            case MenEntering:
                stateColour = ANSI_GREEN;
                statePadded = " Men Entering ";
                break;
            case MenLeaving:
                stateColour = ANSI_RED;
                statePadded = " Men Leaving  ";
                break;
            case WomenEntering:
                stateColour = ANSI_GREEN;
                statePadded = "Women Entering";
                break;
            case WomenLeaving:
                stateColour = ANSI_RED;
                statePadded = "Women Leaving ";
                break;
        }

        String stateString = stateColour + "State: [" + statePadded + "] " + ANSI_RESET;

        String menQueue = "[";
        for (int i = 0; i < menInQueue; i++)
            menQueue += BLACK_SQUARE;
        for (int i = 0; i < numMen - menInQueue; i++)
            menQueue += WHITE_SQUARE;
        menQueue += "]";

        String womenQueue = "[";
        for (int i = 0; i < womenInQueue; i++)
            womenQueue += BLACK_SQUARE;
        for (int i = 0; i < numWomen - womenInQueue; i++)
            womenQueue += WHITE_SQUARE;
        womenQueue += "]";

        String menBathroom = "[";
        for (int i = 0; i < menInBathroom; i++)
            menBathroom += BLACK_SQUARE;
        for (int i = 0; i < numMen - menInBathroom; i++)
            menBathroom += ' ';
        menBathroom += "]";

        String womenBathroom = "[";
        for (int i = 0; i < womenInBathroom; i++)
            womenBathroom += BLACK_SQUARE;
        for (int i = 0; i < numWomen - womenInBathroom; i++)
            womenBathroom += ' ';
        womenBathroom += "]";

        System.out.print(timeStamp + stateString + "Bathroom: " + ANSI_BLUE + "M:" + menBathroom + ANSI_PURPLE + "W:" + womenBathroom + ANSI_RESET + " Queues: " + ANSI_BLUE + "M:" + menQueue + ANSI_PURPLE + "W:" + womenQueue + ANSI_RESET + " ");

    }


}