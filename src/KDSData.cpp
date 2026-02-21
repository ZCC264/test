#include "KDSData.hpp"

KDSData::KDSData(long id) : id_(id) {}

void KDSData::setId(long id) {
    id_ = id;
}

long KDSData::getId() const {
    return id_;
}

void KDSData::addProperty(const std::string& key, const std::string& value) {
    properties_[key] = value;
}

std::string KDSData::getProperty(const std::string& key) const {
    auto it = properties_.find(key);
    if (it != properties_.end()) {
        return it->second;
    }
    return "";
}

void KDSData::removeProperty(const std::string& key) {
    properties_.erase(key);
}

const std::map<std::string, std::string>& KDSData::getProperties() const {
    return properties_;
}