#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>

class Kronos {
public:
    Kronos(const std::string& startRegex, const std::string& completeRegex);

    void initLog(const std::string& readFile);
    void analyzeLog();
    boost::posix_time::ptime getTimestamp(const std::string& line);

    void setStartupRegex(const std::string& regex);
    void setCompleteRegex(const std::string& regex);
    const std::vector<std::string>& getLogLines() const;

private:
    std::vector<std::string> logLines;
    std::regex startup;
    std::regex complete;
};
