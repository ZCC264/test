#ifndef KDSAREA_HPP
#define KDSAREA_HPP

#include <memory>
#include <vector>

#include "KDSData.hpp"
#include "KDCoord.hpp"

class KDSArea : public KDSData {
public:
    KDSArea() = default;

    void addCoord(double x, double y, double z);
    void addCoord(const std::shared_ptr<KDCoord>& coord);
    void clearCoords();

    const std::vector<std::shared_ptr<KDCoord>>& getCoords() const;

private:
    std::vector<std::shared_ptr<KDCoord>> coords_;
};

#endif // KDSAREA_HPP