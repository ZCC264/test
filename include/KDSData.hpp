#ifndef KDSDATA_HPP
#define KDSDATA_HPP

#include <map>
#include <string>

class KDSData {
public:
    explicit KDSData(long id = 0L);
    virtual ~KDSData() = default;

    void setId(long id);
    long getId() const;

    void addProperty(const std::string& key, const std::string& value);
    std::string getProperty(const std::string& key) const;
    void removeProperty(const std::string& key);
    const std::map<std::string, std::string>& getProperties() const;

protected:
    long id_;
    std::map<std::string, std::string> properties_;
};

#endif // KDSDATA_HPP