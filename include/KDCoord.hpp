#ifndef KDCOORD_HPP
#define KDCOORD_HPP

class KDCoord {
public:
    KDCoord();
    KDCoord(double x, double y, double z);

    void setX(double x);
    void setY(double y);
    void setZ(double z);

    double getX() const;
    double getY() const;
    double getZ() const;

private:
    double x_;
    double y_;
    double z_;
};

#endif // KDCOORD_HPP