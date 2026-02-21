#include "KDSGMLExporter.hpp"

#include <fstream>
#include <sstream>

#include "KDSArea.hpp"
#include "KDSNode.hpp"
#include "KDSWay.hpp"

namespace {
std::string xmlEscape(const std::string& value) {
    std::string escaped;
    escaped.reserve(value.size());

    for (const char ch : value) {
        switch (ch) {
            case '&':
                escaped += "&amp;";
                break;
            case '<':
                escaped += "&lt;";
                break;
            case '>':
                escaped += "&gt;";
                break;
            case '\"':
                escaped += "&quot;";
                break;
            case '\'':
                escaped += "&apos;";
                break;
            default:
                escaped += ch;
                break;
        }
    }

    return escaped;
}

std::string posText(const std::shared_ptr<KDCoord>& coord) {
    if (!coord) {
        return "";
    }

    std::ostringstream stream;
    stream << coord->getX() << " " << coord->getY() << " " << coord->getZ();
    return stream.str();
}

std::string posListText(const std::vector<std::shared_ptr<KDCoord>>& coords, bool closeRing) {
    std::ostringstream stream;
    bool first = true;

    for (const auto& coord : coords) {
        if (!coord) {
            continue;
        }

        if (!first) {
            stream << " ";
        }
        stream << posText(coord);
        first = false;
    }

    if (closeRing && !coords.empty() && coords.front()) {
        if (!first) {
            stream << " ";
        }
        stream << posText(coords.front());
    }

    return stream.str();
}

void appendCommonFields(std::ostringstream& gml, const std::shared_ptr<KDSData>& data) {
    if (!data) {
        return;
    }

    gml << "      <kds:id>" << data->getId() << "</kds:id>\n";
    gml << "      <kds:properties>\n";
    for (const auto& property : data->getProperties()) {
        gml << "        <kds:property key=\"" << xmlEscape(property.first) << "\">"
            << xmlEscape(property.second) << "</kds:property>\n";
    }
    gml << "      </kds:properties>\n";
}

void appendHeader(std::ostringstream& gml) {
    gml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    gml << "<gml:FeatureCollection xmlns:gml=\"http://www.opengis.net/gml\" xmlns:kds=\"http://example.com/kds\">\n";
}

void appendFooter(std::ostringstream& gml) {
    gml << "</gml:FeatureCollection>\n";
}

void appendNodes(std::ostringstream& gml, const KDSGMLExporter::DataMap& nodes) {
    for (const auto& pair : nodes) {
        const auto& node = std::dynamic_pointer_cast<KDSNode>(pair.second);
        if (!node || !node->getCoord()) {
            continue;
        }

        const long featureId = node->getId() != 0L ? node->getId() : pair.first;

        gml << "  <gml:featureMember>\n";
        gml << "    <kds:Node gml:id=\"node_" << featureId << "\">\n";
        appendCommonFields(gml, pair.second);
        gml << "      <gml:Point>\n";
        gml << "        <gml:pos>" << posText(node->getCoord()) << "</gml:pos>\n";
        gml << "      </gml:Point>\n";
        gml << "    </kds:Node>\n";
        gml << "  </gml:featureMember>\n";
    }
}

void appendWays(std::ostringstream& gml, const KDSGMLExporter::DataMap& ways) {
    for (const auto& pair : ways) {
        const auto& way = std::dynamic_pointer_cast<KDSWay>(pair.second);
        if (!way || way->getCoords().empty()) {
            continue;
        }

        const long featureId = way->getId() != 0L ? way->getId() : pair.first;

        gml << "  <gml:featureMember>\n";
        gml << "    <kds:Way gml:id=\"way_" << featureId << "\">\n";
        appendCommonFields(gml, pair.second);
        gml << "      <gml:LineString>\n";
        gml << "        <gml:posList>" << posListText(way->getCoords(), false) << "</gml:posList>\n";
        gml << "      </gml:LineString>\n";
        gml << "    </kds:Way>\n";
        gml << "  </gml:featureMember>\n";
    }
}

void appendAreas(std::ostringstream& gml, const KDSGMLExporter::DataMap& areas) {
    for (const auto& pair : areas) {
        const auto& area = std::dynamic_pointer_cast<KDSArea>(pair.second);
        if (!area || area->getCoords().size() < 3) {
            continue;
        }

        const long featureId = area->getId() != 0L ? area->getId() : pair.first;

        gml << "  <gml:featureMember>\n";
        gml << "    <kds:Area gml:id=\"area_" << featureId << "\">\n";
        appendCommonFields(gml, pair.second);
        gml << "      <gml:Polygon>\n";
        gml << "        <gml:exterior>\n";
        gml << "          <gml:LinearRing>\n";
        gml << "            <gml:posList>" << posListText(area->getCoords(), true) << "</gml:posList>\n";
        gml << "          </gml:LinearRing>\n";
        gml << "        </gml:exterior>\n";
        gml << "      </gml:Polygon>\n";
        gml << "    </kds:Area>\n";
        gml << "  </gml:featureMember>\n";
    }
}

std::string joinPath(const std::string& directory, const std::string& fileName) {
    if (directory.empty()) {
        return fileName;
    }

    const char lastChar = directory.back();
    if (lastChar == '/' || lastChar == '\\') {
        return directory + fileName;
    }

    return directory + "/" + fileName;
}

bool writeTextFile(const std::string& filePath, const std::string& content) {
    std::ofstream output(filePath);
    if (!output.is_open()) {
        return false;
    }

    output << content;
    return true;
}
}

std::string KDSGMLExporter::toGMLNodes(const DataMap& nodes) const {
    std::ostringstream gml;
    appendHeader(gml);
    appendNodes(gml, nodes);
    appendFooter(gml);
    return gml.str();
}

std::string KDSGMLExporter::toGMLWays(const DataMap& ways) const {
    std::ostringstream gml;
    appendHeader(gml);
    appendWays(gml, ways);
    appendFooter(gml);
    return gml.str();
}

std::string KDSGMLExporter::toGMLAreas(const DataMap& areas) const {
    std::ostringstream gml;
    appendHeader(gml);
    appendAreas(gml, areas);
    appendFooter(gml);
    return gml.str();
}

bool KDSGMLExporter::writeByTypeFiles(const std::string& outputDirectory, const DataMap& nodes, const DataMap& ways, const DataMap& areas) const {
    const std::string nodesPath = joinPath(outputDirectory, "nodes.gml");
    const std::string waysPath = joinPath(outputDirectory, "ways.gml");
    const std::string areasPath = joinPath(outputDirectory, "areas.gml");

    const bool nodeOk = writeTextFile(nodesPath, toGMLNodes(nodes));
    const bool wayOk = writeTextFile(waysPath, toGMLWays(ways));
    const bool areaOk = writeTextFile(areasPath, toGMLAreas(areas));

    return nodeOk && wayOk && areaOk;
}

std::string KDSGMLExporter::toGML(const DataMap& nodes, const DataMap& ways, const DataMap& areas) const {
    std::ostringstream gml;
    appendHeader(gml);
    appendNodes(gml, nodes);
    appendWays(gml, ways);
    appendAreas(gml, areas);
    appendFooter(gml);
    return gml.str();
}

bool KDSGMLExporter::writeToFile(const std::string& filePath, const DataMap& nodes, const DataMap& ways, const DataMap& areas) const {
    return writeTextFile(filePath, toGML(nodes, ways, areas));
}
