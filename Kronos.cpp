#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "Kronos.hpp"

Kronos::Kronos(const std::string& startRegex, const std::string& completeRegex) {
    setStartupRegex(startRegex);
    setCompleteRegex(completeRegex);
}

void Kronos::initLog(const std::string& readFile) {
    std::string line;
    std::ifstream file(readFile);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to access file for processing!");
    }
    while (std::getline(file, line)) {
        logLines.push_back(line);
    }

    file.close();
}

boost::posix_time::ptime Kronos::getTimestamp(const std::string& line) {
    std::regex timestamp_regex(R"(\d{4}-\d{2}-\d{2}\s\d{2}:\d{2}:\d{2})");
    std::smatch match;

    if (std::regex_search(line, match, timestamp_regex)) {
        std::string timestamp_str = match.str();
        boost::posix_time::ptime timestamp = boost::posix_time::time_from_string(timestamp_str);
        return timestamp;
    }
    else {
        throw std::runtime_error("Timestamp not found in line");
    }
}

void Kronos::setStartupRegex(const std::string& regex) {
    startup = std::regex(regex);
}

void Kronos::setCompleteRegex(const std::string& regex) {
    complete = std::regex(regex);
}

const std::vector<std::string>& Kronos::getLogLines() const {
    return logLines;
}

void Kronos::analyzeLog() {
    bool startFound = false;
    boost::posix_time::ptime startTime;
    size_t startLineNumber = 0;

    for (size_t lineNumber = 0; lineNumber < logLines.size(); ++lineNumber) {
        const std::string& line = logLines[lineNumber];

        if (!startFound && std::regex_search(line, startup)) {
            startFound = true;
            startTime = getTimestamp(line);
            startLineNumber = lineNumber;
        }
        else if (startFound && std::regex_search(line, complete)) {
            boost::posix_time::ptime completeTime = getTimestamp(line);
            boost::posix_time::time_duration duration = completeTime - startTime;

            std::cout << startLineNumber << ": " << startTime << " server started" << std::endl;
            std::cout << "Success: Time elapsed: " << duration << std::endl << std::endl;

            startFound = false;
        }
        else if (startFound && std::regex_search(line, startup)) {
            std::cout << "Failure: Line " << startLineNumber << ": " << startTime << " server started" << std::endl;
            std::cout << std::endl;
            startFound = false;
            --lineNumber;
        }
    }

    if (startFound) {
        std::cout << "Failure: Line " << startLineNumber << ": " << startTime << " server started" << std::endl;
    }
}

