/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

public class Quad {

    double xmid;
    double ymid;
    double length;

    public Quad(double xmid, double ymid, double length) {
        this.xmid = xmid;
        this.ymid = ymid;
        this.length = length;
    }

    public double length() {
        return length;
    }

    public boolean contains(double x, double y) {
        double halfLength = this.length / 2;
        return (x <= this.xmid + halfLength
                && x >= this.xmid - halfLength
                && y <= this.ymid + halfLength
                && y >= this.ymid - halfLength);
    }

    public Quad NW() {
        double x = this.xmid - this.length / 4;
        double y = this.ymid + this.length / 4;
        double len = this.length / 2;
        return new Quad(x, y, len);
    }

    public Quad NE() {
        double x = this.xmid + this.length / 4;
        double y = this.ymid + this.length / 4;
        double len = this.length / 2;
        return new Quad(x, y, len);
    }

    public Quad SW() {
        double x = this.xmid - this.length / 4;
        double y = this.ymid - this.length / 4;
        double len = this.length / 2;
        return new Quad(x, y, len);
    }

    public Quad SE() {
        double x = this.xmid + this.length / 4;
        double y = this.ymid - this.length / 4;
        double len = this.length / 2;
        return new Quad(x, y, len);
    }
   
}