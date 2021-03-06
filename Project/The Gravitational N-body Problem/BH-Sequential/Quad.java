public class Quad {
    
    private double xmid;
    private double ymid;
    private double length;   
 
    public Quad(double xmid, double ymid, double length) {
        this.xmid = xmid;
        this.ymid = ymid;
        this.length = length;
    }

    /**
     * Returns the length of one side of the square quadrant.
     *
     * @return side length of the quadrant
     */
    public double length() {
        return length;
    }

    /**
     * Does this quadrant contain (x, y)?
     *
     * @param x x-coordinate of point to test
     * @param y y-coordinate of point to test
     * @return  true if quadrant contains (x, y), else false
     */
    public boolean contains(double x, double y) {
        double halfLen = this.length / 2.0;
        return (x <= this.xmid + halfLen && 
                x >= this.xmid - halfLen &&
                y <= this.ymid + halfLen && 
                y >= this.ymid - halfLen);
    }

    /**
     * Returns a new object that represents the northwest quadrant.
     *
     * @return the northwest quadrant of this Quad
     */
    public Quad NW() {
        double x = this.xmid - this.length / 4.0;
        double y = this.ymid + this.length / 4.0;
        double len = this.length / 2.0;
        Quad NW = new Quad(x, y, len);
        return NW;
    }

    /**
     * Returns a new object that represents the northeast quadrant.
     *
     * @return the northeast quadrant of this Quad
     */
    public Quad NE() {
        double x = this.xmid + this.length / 4.0;
        double y = this.ymid + this.length / 4.0;
        double len = this.length / 2.0;
        Quad NE = new Quad(x, y, len);
        return NE;
    }

    /**
     * Returns a new object that represents the southwest quadrant.
     *
     * @return the southwest quadrant of this Quad
     */
    public Quad SW() {
        double x = this.xmid - this.length / 4.0;
        double y = this.ymid - this.length / 4.0;
        double len = this.length / 2.0;
        Quad SW = new Quad(x, y, len);
        return SW;
    }

    /**
     * Returns a new object that represents the southeast quadrant.
     *
     * @return the southeast quadrant of this Quad
     */
    public Quad SE() {
        double x = this.xmid + this.length / 4.0;
        double y = this.ymid - this.length / 4.0;
        double len = this.length / 2.0;
        Quad SE = new Quad(x, y, len);
        return SE;
    }
}