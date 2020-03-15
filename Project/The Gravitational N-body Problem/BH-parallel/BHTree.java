public class BHTree {

    // threshold value
    private final double Theta = 0.5;

    private Point point;     // body or aggregate body stored in this node
    private Quad quad;     // square region that the tree represents
    private BHTree NW;     // tree representing northwest quadrant
    private BHTree NE;     // tree representing northeast quadrant
    private BHTree SW;     // tree representing southwest quadrant
    private BHTree SE;     // tree representing southeast quadrant
 
    public BHTree(Quad q) {
        this.quad = q;
        this.point = null;
        this.NW = null;
        this.NE = null;
        this.SW = null;
        this.SE = null;
    }
 
    /**
     * Adds the Point p to the invoking Barnes-Hut tree.
     */
    public void insert(Point p) {

//        System.out.println("got into insert");
        // if this node does not contain a body, put the new Point p here
        if (point == null) {
            point = p;
            return;
        }
  
        // internal node
        if (! isExternal()) {
            // update the center-of-mass and total mass
            point = point.plus(p);
        
            // recursively insert Point p into the appropriate quadrant
            putBody(p);
        }

        // external node
        else {
            // subdivide the region further by creating four children
            NW = new BHTree(quad.NW());
            NE = new BHTree(quad.NE());
            SE = new BHTree(quad.SE());
            SW = new BHTree(quad.SW());

            // recursively insert both this body and Point p into the appropriate quadrant
            putBody(this.point);
            putBody(p);

            // update the center-of-mass and total mass
            point = point.plus(p);
        }
    }

    /**
     * Inserts a body into the appropriate quadrant.
     */ 
    private void putBody(Point p) {
        if (p.in(quad.NW()))
            NW.insert(p);
        else if (p.in(quad.NE()))
            NE.insert(p);
        else if (p.in(quad.SE()))
            SE.insert(p);
        else if (p.in(quad.SW()))
            SW.insert(p);
    }

    /**
     * Returns true iff this tree node is external.
     */
    private boolean isExternal() {
        // a node is external iff all four children are null
        return (NW == null && NE == null && SW == null && SE == null);
    }

    /**
     * Approximates the net force acting on Point p from all bodies
     * in the invoking Barnes-Hut tree, and updates p's force accordingly.
     */
    public void updateForce(Point p) {
    
        if (point == null || p.equals(point))
            return;

        // if the current node is external, update net force acting on p
        if (isExternal()) 
            p.addForce(point);
 
        // for internal nodes
        else {
    
            // width of region represented by internal node
            double s = quad.length();

            // distance between Point p and this node's center-of-mass
            double d = point.distanceTo(p);

            // compare ratio (s / d) to threshold value Theta
            if ((s / d) < Theta)
                p.addForce(point);   // p is far away
            
            // recurse on each of current node's children
            else {
                NW.updateForce(p);
                NE.updateForce(p);
                SW.updateForce(p);
                SE.updateForce(p);
            }
        }
    }
}