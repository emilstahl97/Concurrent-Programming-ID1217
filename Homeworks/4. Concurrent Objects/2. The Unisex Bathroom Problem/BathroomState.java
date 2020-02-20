public class BathroomState {

    public BathroomState.State state = BathroomState.State.Empty;
    
    private int time = 0;
    public int menInQueue = 0;
    public int menInBathroom = 0;
    public int womenInQueue = 0;
    public int womenInBathroom = 0;
    
    private int numWomen;
    private int numMen;
    
    public final String WOMAN = "\uD83D\uDEBA";
    public final String MAN = "\uD83D\uDEB9";

    private static final char BLACK_SQUARE = '\u25A0';
    private static final char WHITE_SQUARE = '\u25A1';

    private static final String ANSI_RESET = "\u001B[0m";
    private static final String ANSI_PURPLE = "\u001B[35m";
    private static final String ANSI_BLUE = "\u001B[34m";
    private static final String ANSI_RED = "\u001B[31m";
    private static final String ANSI_GREEN = "\u001B[32m";
    private static final String ANSI_YELLOW = "\u001B[33m";
    

    public enum State {
        
        Empty,
        MenEntering,
        WomenEntering,
        MenLeaving,
        WomenLeaving,
    }
    
    public void setNum(int numWomen, int numMen) {

        this.numWomen = numWomen;
        this.numMen = numMen;
    }

    public void printQueues() {
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

        String womenQueue = "[";
        for (int i = 0; i < womenInQueue; i++)
            womenQueue += BLACK_SQUARE;
        for (int i = 0; i < numWomen - womenInQueue; i++)
            womenQueue += WHITE_SQUARE;
        womenQueue += "]";

        String menQueue = "[";
        for (int i = 0; i < menInQueue; i++)
            menQueue += BLACK_SQUARE;
        for (int i = 0; i < numMen - menInQueue; i++)
            menQueue += WHITE_SQUARE;
        menQueue += "]";

        String womenBathroom = "[";
        for (int i = 0; i < womenInBathroom; i++)
            womenBathroom += BLACK_SQUARE;
        for (int i = 0; i < (numWomen - womenInBathroom); i++)
            womenBathroom += ' ';
        womenBathroom += "]";

        String menBathroom = "[";
        for (int i = 0; i < menInBathroom; i++)
            menBathroom += BLACK_SQUARE;
        for (int i = 0; i < numMen - menInBathroom; i++)
            menBathroom += ' ';
        menBathroom += "]";

        System.out.print(timeStamp + stateString + "Bathroom: " + ANSI_PURPLE + "W:" + womenBathroom + ANSI_BLUE + "M:" + menBathroom + ANSI_RESET + " Queues: " + ANSI_PURPLE + "W:" + womenQueue + ANSI_BLUE + "M:" + menQueue + ANSI_RESET + " ");
    }
}