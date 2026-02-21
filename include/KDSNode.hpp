#ifndef KDSNODE_HPP
#define KDSNODE_HPP

#include <memory>

#include "KDSData.hpp"
#include "KDCoord.hpp"

class KDSNode : public KDSData {
public:
    KDSNode(double x = 0.0, double y = 0.0, double z = 0.0);
    explicit KDSNode(const std::shared_ptr<KDCoord>& coord);

    void setCoord(double x, double y, double z);
    void setCoord(const std::shared_ptr<KDCoord>& coord);

    std::shared_ptr<KDCoord> getCoord() const;

private:
    std::shared_ptr<KDCoord> coord_;
};

#endif // KDSNODE_HPP