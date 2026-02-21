#ifndef KDSWAY_HPP
#define KDSWAY_HPP

#include <memory>
#include <vector>

#include "KDSData.hpp"
#include "KDCoord.hpp"

class KDSWay : public KDSData {
public:
    KDSWay() = default;

    void addCoord(double x, double y, double z);
    void addCoord(const std::shared_ptr<KDCoord>& coord);
    void clearCoords();

    const std::vector<std::shared_ptr<KDCoord>>& getCoords() const;

private:
    std::vector<std::shared_ptr<KDCoord>> coords_;
};

#endif // KDSWAY_HPP