public class Point {

    double posX;
    double posY;
    double velX;
    double velY;
    double[] forcesX;
    double[] forcesY;
    double mass;

    public Point(double posX, double posY, double velX, double velY, double forcesX, double forcesY, double mass, int numWorkers) {
        
        this.posX = posX;
        this.posY = posY;
        this.velX = velX;
        this.velY = velY;
        this.mass = mass;
        
        this.forcesX = new double [numWorkers];
        this.forcesY = new double [numWorkers];
        
        for(int i = 0; i < numWorkers; i++){
            
            this.forcesX[i] = forcesX;
            this.forcesY[i] = forcesY;
        }
    }
}