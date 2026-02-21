#include "KDSWay.hpp"

void KDSWay::addCoord(double x, double y, double z) {
    coords_.push_back(std::make_shared<KDCoord>(x, y, z));
}

void KDSWay::addCoord(const std::shared_ptr<KDCoord>& coord) {
    if (coord) {
        coords_.push_back(coord);
    }
}

void KDSWay::clearCoords() {
    coords_.clear();
}

const std::vector<std::shared_ptr<KDCoord>>& KDSWay::getCoords() const {
    return coords_;
}
