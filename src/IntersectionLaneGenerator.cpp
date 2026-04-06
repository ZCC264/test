#include "IntersectionLaneGenerator.hpp"

#include <cmath>
#include <stdexcept>

namespace {

// Bézier control-point offset as a fraction of the chord length.
// A value of 1/3 keeps a straight-through curve very close to a straight line.
const double STRAIGHT_CONTROL_SCALE  = 0.33;
// Wider arcs are typical for turns; left turns tend to be slightly wider
// than right turns because they cross more of the intersection.
const double LEFT_TURN_CONTROL_SCALE  = 0.55;
const double RIGHT_TURN_CONTROL_SCALE = 0.45;

// Minimum segment length below which a tangent is considered degenerate.
const double EPSILON_LENGTH = 1e-10;

} // namespace

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

IntersectionLaneGenerator::IntersectionLaneGenerator(int numSamples)
    : numSamples_(numSamples)
{
    if (numSamples_ < 2) {
        throw std::invalid_argument("numSamples must be >= 2");
    }
}

// ---------------------------------------------------------------------------
// Public interface
// ---------------------------------------------------------------------------

std::vector<KDCoord> IntersectionLaneGenerator::generateStraight(
    const std::vector<KDCoord>& enterNodes,
    const std::vector<KDCoord>& exitNodes) const
{
    // A relatively small control-point offset keeps the curve close to a
    // straight line while still being tangentially continuous.
    return computeBezier(enterNodes, exitNodes, STRAIGHT_CONTROL_SCALE);
}

std::vector<KDCoord> IntersectionLaneGenerator::generateLeftTurn(
    const std::vector<KDCoord>& enterNodes,
    const std::vector<KDCoord>& exitNodes) const
{
    // A larger offset produces a fuller arc, typical for a turn.
    return computeBezier(enterNodes, exitNodes, LEFT_TURN_CONTROL_SCALE);
}

std::vector<KDCoord> IntersectionLaneGenerator::generateRightTurn(
    const std::vector<KDCoord>& enterNodes,
    const std::vector<KDCoord>& exitNodes) const
{
    // Right turns are generally tighter; a moderate offset reflects that.
    return computeBezier(enterNodes, exitNodes, RIGHT_TURN_CONTROL_SCALE);
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

std::vector<KDCoord> IntersectionLaneGenerator::computeBezier(
    const std::vector<KDCoord>& enterNodes,
    const std::vector<KDCoord>& exitNodes,
    double controlScale) const
{
    if (enterNodes.size() < 2) {
        throw std::invalid_argument("enterNodes must contain at least 2 points");
    }
    if (exitNodes.size() < 2) {
        throw std::invalid_argument("exitNodes must contain at least 2 points");
    }

    const KDCoord& p0 = enterNodes.back();   // entry point
    const KDCoord& p3 = exitNodes.front();   // exit  point

    // Chord length in XY (Z is linearly interpolated).
    const double dx = p3.getX() - p0.getX();
    const double dy = p3.getY() - p0.getY();
    const double chord = std::sqrt(dx * dx + dy * dy);

    // Tangent directions at p0 and p3.
    const KDCoord tEnter = tailTangent(enterNodes);
    const KDCoord tExit  = headTangent(exitNodes);

    // Control points.
    const double offset = controlScale * chord;

    const KDCoord p1(
        p0.getX() + tEnter.getX() * offset,
        p0.getY() + tEnter.getY() * offset,
        p0.getZ() + tEnter.getZ() * offset);

    const KDCoord p2(
        p3.getX() - tExit.getX() * offset,
        p3.getY() - tExit.getY() * offset,
        p3.getZ() - tExit.getZ() * offset);

    // Sample the curve.
    std::vector<KDCoord> result;
    result.reserve(static_cast<std::size_t>(numSamples_));

    for (int i = 0; i < numSamples_; ++i) {
        const double t = static_cast<double>(i) / static_cast<double>(numSamples_ - 1);
        result.push_back(cubicBezier(p0, p1, p2, p3, t));
    }

    return result;
}

KDCoord IntersectionLaneGenerator::tailTangent(const std::vector<KDCoord>& nodes)
{
    const KDCoord& a = nodes[nodes.size() - 2];
    const KDCoord& b = nodes[nodes.size() - 1];

    const double dx = b.getX() - a.getX();
    const double dy = b.getY() - a.getY();
    const double dz = b.getZ() - a.getZ();
    const double len = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (len < EPSILON_LENGTH) {
        return KDCoord(1.0, 0.0, 0.0);  // fallback: positive-X direction
    }
    return KDCoord(dx / len, dy / len, dz / len);
}

KDCoord IntersectionLaneGenerator::headTangent(const std::vector<KDCoord>& nodes)
{
    const KDCoord& a = nodes[0];
    const KDCoord& b = nodes[1];

    const double dx = b.getX() - a.getX();
    const double dy = b.getY() - a.getY();
    const double dz = b.getZ() - a.getZ();
    const double len = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (len < EPSILON_LENGTH) {
        return KDCoord(1.0, 0.0, 0.0);  // fallback: positive-X direction
    }
    return KDCoord(dx / len, dy / len, dz / len);
}

KDCoord IntersectionLaneGenerator::cubicBezier(
    const KDCoord& p0, const KDCoord& p1,
    const KDCoord& p2, const KDCoord& p3,
    double t)
{
    const double u  = 1.0 - t;
    const double u2 = u  * u;
    const double u3 = u2 * u;
    const double t2 = t  * t;
    const double t3 = t2 * t;

    const double x = u3 * p0.getX()
                   + 3.0 * u2 * t  * p1.getX()
                   + 3.0 * u  * t2 * p2.getX()
                   +            t3 * p3.getX();

    const double y = u3 * p0.getY()
                   + 3.0 * u2 * t  * p1.getY()
                   + 3.0 * u  * t2 * p2.getY()
                   +            t3 * p3.getY();

    const double z = u3 * p0.getZ()
                   + 3.0 * u2 * t  * p1.getZ()
                   + 3.0 * u  * t2 * p2.getZ()
                   +            t3 * p3.getZ();

    return KDCoord(x, y, z);
}
