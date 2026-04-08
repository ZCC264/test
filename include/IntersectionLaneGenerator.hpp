#ifndef INTERSECTION_LANE_GENERATOR_HPP
#define INTERSECTION_LANE_GENERATOR_HPP

#include <vector>
#include "KDCoord.hpp"

/**
 * Generates lane centerlines inside an intersection.
 *
 * Each method accepts:
 *   enterNodes - ordered centerline nodes of the lane entering the intersection
 *   exitNodes  - ordered centerline nodes of the lane exiting the intersection
 *
 * and returns a new ordered list of KDCoord points that form the centerline
 * connecting the two lanes through the intersection.
 *
 * The curves are computed with cubic Bézier interpolation so that the
 * generated centerline is tangentially continuous with both input lanes.
 */
class IntersectionLaneGenerator {
public:
    /**
     * @param numSamples  Number of points to sample along the generated curve
     *                    (including start and end). Must be >= 2.
     */
    explicit IntersectionLaneGenerator(int numSamples = 20);

    /**
     * Generate a straight-through lane centerline.
     *
     * @param enterNodes  Centerline nodes of the entering lane (at least 2 points).
     * @param exitNodes   Centerline nodes of the exiting  lane (at least 2 points).
     * @return            Sampled centerline nodes inside the intersection.
     */
    std::vector<KDCoord> generateStraight(
        const std::vector<KDCoord>& enterNodes,
        const std::vector<KDCoord>& exitNodes) const;

    /**
     * Generate a left-turn lane centerline.
     *
     * @param enterNodes  Centerline nodes of the entering lane (at least 2 points).
     * @param exitNodes   Centerline nodes of the exiting  lane (at least 2 points).
     * @return            Sampled centerline nodes inside the intersection.
     */
    std::vector<KDCoord> generateLeftTurn(
        const std::vector<KDCoord>& enterNodes,
        const std::vector<KDCoord>& exitNodes) const;

    /**
     * Generate a right-turn lane centerline.
     *
     * @param enterNodes  Centerline nodes of the entering lane (at least 2 points).
     * @param exitNodes   Centerline nodes of the exiting  lane (at least 2 points).
     * @return            Sampled centerline nodes inside the intersection.
     */
    std::vector<KDCoord> generateRightTurn(
        const std::vector<KDCoord>& enterNodes,
        const std::vector<KDCoord>& exitNodes) const;

private:
    int numSamples_;

    /**
     * Core cubic Bézier sampler.
     *
     * P0 = last point of enterNodes
     * P3 = first point of exitNodes
     * P1 = P0 + tangent_enter * controlScale * chord
     * P2 = P3 - tangent_exit  * controlScale * chord
     *
     * @param controlScale  Fraction of chord length used for control-point offset.
     *                      Larger values produce a "fuller" curve; use ~0.33 for
     *                      straight, larger (~0.5) for wider turns.
     */
    std::vector<KDCoord> computeBezier(
        const std::vector<KDCoord>& enterNodes,
        const std::vector<KDCoord>& exitNodes,
        double controlScale) const;

    /** Unit tangent at the tail of a node sequence (last two points). */
    static KDCoord tailTangent(const std::vector<KDCoord>& nodes);

    /** Unit tangent at the head of a node sequence (first two points). */
    static KDCoord headTangent(const std::vector<KDCoord>& nodes);

    /** Evaluate a cubic Bézier at parameter t in [0, 1]. */
    static KDCoord cubicBezier(
        const KDCoord& p0, const KDCoord& p1,
        const KDCoord& p2, const KDCoord& p3,
        double t);
};

#endif // INTERSECTION_LANE_GENERATOR_HPP
