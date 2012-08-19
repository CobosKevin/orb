#ifndef MEGABRITE_H
#define MEGABRITE_H

#define RED     0
#define GREEN   1
#define BLUE    2

#define SLOW    0
#define FAST    1

class MegaBrite
{
public:
    MegaBrite();
    void setup();
    void pulseOn(unsigned short led, unsigned short speed);
    void pulseOff(unsigned short led);
    void toggleOn(unsigned short led);
    void toggleOff(unsigned short led);
    void on(unsigned short led);
    void off(unsigned short led);
    void animate();

private:
    int value[3];
    bool pulse[3];
    bool toggle[3];
    unsigned short step[3];
    int direction[3];

    void setPWMRegister();
    void setControlRegister();
    void sendPacket(int mode, int red, int green, int blue);

    int previousRed;
    int previousGreen;
    int previousBlue;

};

#endif
