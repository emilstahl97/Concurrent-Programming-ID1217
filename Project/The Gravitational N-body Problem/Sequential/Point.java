public class Point {
    
    double posX;
    double posY;
    double velX;
    double velY; 
    double forceX;
    double forceY;
    double mass; 
    
 
    public Point(double posX, double posY, double velX, double velY, double forceX, double forceY, double mass){

        this.posX = posX; 
        this.posY = posY;
        this.velX = velX; 
        this.velY = velY;
        this.forceX = forceX; 
        this.forceY  = forceY;
        this.mass = mass; 
    }
}