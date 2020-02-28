
public class BHTree {

    Quad quad;
    Point point;
    BHTree NW;
    BHTree NE;
    BHTree SW;
    BHTree SE;

    private double theta = 0.8;

    public BHTree(Quad quad, double theta) {
        this.quad = quad;
        this.theta = theta; 
    }

    //if all four quads is == NULL
    private boolean isExternal() {
        return (NW == null && NE == null && SW == null && SE == null);
    }

    public void calculateForce(Point P) {
        if (point == null || point.equals(P)) {
            return;
        }

        if (isExternal()) {
            P.addForce(point);
        } else {
            double width = quad.length;
            double distance = point.distance(P);

            if ((width / distance < theta)) {
                P.addForce(point);
            } else {
                NW.calculateForce(P);
                NE.calculateForce(P);
                SW.calculateForce(P);
                SE.calculateForce(P);
            }
        }
    }

    public void insertPoint(Point p) {
        if (point == null) {
            point = p;
            return;
        }

        if (!isExternal()) {
            point = point.addMasses(p);
            putPoint(point);
        } else {
            NW = new BHTree(quad.NW(), theta);
            NE = new BHTree(quad.NE(), theta);
            SE = new BHTree(quad.SE(), theta);
            SW = new BHTree(quad.SW(), theta);
            putPoint(this.point);
            putPoint(p);
            point = point.addMasses(p);
        }

    }

    private void putPoint(Point p) {
        if (p.inQuad(quad.NW())) {
            NW.insertPoint(p);
        } else if (p.inQuad(quad.NE())) {
            NE.insertPoint(p);
        } else if (p.inQuad(quad.SE())) {
            SE.insertPoint(p);
        } else if (p.inQuad(quad.SW())) {
            SW.insertPoint(p);
        }
    }
}