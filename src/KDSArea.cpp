#include "KDSArea.hpp"

void KDSArea::addCoord(double x, double y, double z) {
    coords_.push_back(std::make_shared<KDCoord>(x, y, z));
}

void KDSArea::addCoord(const std::shared_ptr<KDCoord>& coord) {
    if (coord) {
        coords_.push_back(coord);
    }
}

void KDSArea::clearCoords() {
    coords_.clear();
}

const std::vector<std::shared_ptr<KDCoord>>& KDSArea::getCoords() const {
    return coords_;
}
