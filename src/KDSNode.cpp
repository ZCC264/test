#include "KDSNode.hpp"

KDSNode::KDSNode(double x, double y, double z)
    : coord_(std::make_shared<KDCoord>(x, y, z)) {}

KDSNode::KDSNode(const std::shared_ptr<KDCoord>& coord)
    : coord_(coord ? coord : std::make_shared<KDCoord>(0.0, 0.0, 0.0)) {}

void KDSNode::setCoord(double x, double y, double z) {
    if (!coord_) {
        coord_ = std::make_shared<KDCoord>(x, y, z);
        return;
    }

    coord_->setX(x);
    coord_->setY(y);
    coord_->setZ(z);
}

void KDSNode::setCoord(const std::shared_ptr<KDCoord>& coord) {
    coord_ = coord ? coord : std::make_shared<KDCoord>(0.0, 0.0, 0.0);
}

std::shared_ptr<KDCoord> KDSNode::getCoord() const {
    return coord_;
}
