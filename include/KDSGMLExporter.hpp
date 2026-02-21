#ifndef KDSGMLEXPORTER_HPP
#define KDSGMLEXPORTER_HPP

#include <map>
#include <memory>
#include <string>

#include "KDSData.hpp"

class KDSGMLExporter {
public:
    using DataMap = std::map<long, std::shared_ptr<KDSData>>;

    std::string toGMLNodes(const DataMap& nodes) const;
    std::string toGMLWays(const DataMap& ways) const;
    std::string toGMLAreas(const DataMap& areas) const;
    bool writeByTypeFiles(const std::string& outputDirectory, const DataMap& nodes, const DataMap& ways, const DataMap& areas) const;

    std::string toGML(const DataMap& nodes, const DataMap& ways, const DataMap& areas) const;
    bool writeToFile(const std::string& filePath, const DataMap& nodes, const DataMap& ways, const DataMap& areas) const;
};

#endif // KDSGMLEXPORTER_HPP
