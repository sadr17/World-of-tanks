#ifndef TANK_H
#define TANK_H

class Tank
{
public:
    Tank();
    Tank(int i, double x = 0, double y = 0, double rot = 0);
    void setRotation(double value);
    double getRotation();
    void setPos(double x, double y);
    double getXPos();
    double getYPos();
    double getCannonRotation();
    void setCannonRotation(double value);
    int id;
private:
    double xPos, yPos;
    double rotation, cannonRotation;
};

#endif // TANK_H
