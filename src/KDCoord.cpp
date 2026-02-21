#include "KDCoord.hpp"

KDCoord::KDCoord() : x_(0.0), y_(0.0), z_(0.0) {}

KDCoord::KDCoord(double x, double y, double z) : x_(x), y_(y), z_(z) {}

void KDCoord::setX(double x) {
    x_ = x;
}

void KDCoord::setY(double y) {
    y_ = y;
}

void KDCoord::setZ(double z) {
    z_ = z;
}

double KDCoord::getX() const {
    return x_;
}

double KDCoord::getY() const {
    return y_;
}

double KDCoord::getZ() const {
    return z_;
}