#include <iostream>
#include <map>
#include <memory>

#include "KDSArea.hpp"
#include "KDSGMLExporter.hpp"
#include "KDSNode.hpp"
#include "KDSWay.hpp"

int main() {
    auto node = std::make_shared<KDSNode>(1.0, 2.0, 3.0);
    node->setId(1001L);
    node->addProperty("type", "node");
    node->addProperty("name", "N-1");
    node->addProperty("note", "A&B <test>");

    auto way = std::make_shared<KDSWay>();
    way->setId(2001L);
    way->addProperty("type", "way");
    way->addProperty("name", "MainRoad");
    way->addProperty("speed", "60");
    way->addCoord(0.0, 0.0, 0.0);
    way->addCoord(1.0, 1.0, 0.0);
    way->addCoord(2.0, 1.0, 0.0);

    auto area = std::make_shared<KDSArea>();
    area->setId(3001L);
    area->addProperty("type", "area");
    area->addProperty("name", "CentralPark");
    area->addProperty("usage", "public");
    area->addCoord(0.0, 0.0, 0.0);
    area->addCoord(3.0, 0.0, 0.0);
    area->addCoord(3.0, 3.0, 0.0);
    area->addCoord(0.0, 3.0, 0.0);

    std::map<long, std::shared_ptr<KDSData>> nodes;
    std::map<long, std::shared_ptr<KDSData>> ways;
    std::map<long, std::shared_ptr<KDSData>> areas;

    nodes[node->getId()] = node;
    ways[way->getId()] = way;
    areas[area->getId()] = area;

    KDSGMLExporter exporter;
    const std::string gml = exporter.toGML(nodes, ways, areas);

    std::cout << gml << std::endl;

    if (exporter.writeToFile("output.gml", nodes, ways, areas)) {
        std::cout << "GML written to output.gml" << std::endl;
    } else {
        std::cout << "Failed to write output.gml" << std::endl;
    }

    return 0;
}
