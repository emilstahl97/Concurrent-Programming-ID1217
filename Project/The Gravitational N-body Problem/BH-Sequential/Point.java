
public class Point {

    double G = 6.67e-11;
    double DT = 0.1;

    double posX;
    double posY;
    double velX;
    double velY;
    double forceX;
    double forceY;
    double mass;

    public Point(double posX, double posY, double velX, double velY,
            double forceX, double forceY, double mass) {
        this.posX = posX;
        this.posY = posY;
        this.velX = velX;
        this.velY = velY;
        this.forceX = forceX;
        this.forceY = forceY;
        this.mass = mass;
    }

    public boolean inQuad(Quad q) {
        return q.contains(posX, posY);
    }

    public void addForce(Point p) {
        double delX = p.posX - this.posX;
        double delY = p.posY - this.posY;
        double distance = this.distance(p);
        
        if(distance > 1){
            distance = 1;
        }

        double force = (G*this.mass*p.mass)/(distance*distance);
        this.forceX += force*delX/distance;
        this.forceY += force*delY/distance;
    }

    public void movePoint() {
        velX += DT*forceX/mass; 
        velY += DT*forceY/mass;
        posX += DT * velX; 
        posY += DT * velY;

    }

    public double distance(Point p) {
        return Math.sqrt(Math.pow((this.posX - p.posX), 2) + Math.pow((this.posY - p.posY), 2));
    }

    public Point addMasses(Point p) {
        Point a = this;

        double m = a.mass + p.mass;
        double x = (a.posX * a.mass + p.posX * p.mass) / m;
        double y = (a.posY * a.mass + p.posY * p.mass) / m;

        return new Point(x, y, a.velX, p.velY, 0, 0, m);
    }
}