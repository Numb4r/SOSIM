#pragma once

#include "fileIO.hpp"
#include "json.hpp"
#include <list>
#include <map>
#include <queue>
#include <sstream>
#include <string>
namespace logps {
enum schedulerPolicy { FIFO, LRU ,MLQ};
enum priorities { realtime, high, medium, low, zero };
enum resources { cpu, ram, disk };

} // namespace logps
struct Logps {
  std::map<logps::priorities, std::string> tPriorities = {
      {logps::priorities::realtime, "realtime"},
      {logps::priorities::high, "high"},
      {logps::priorities::medium, "medium"},
      {logps::priorities::low, "low"},
      {logps::priorities::zero, "zero"}};
  std::map<logps::resources, std::string> tResources = {
      {logps::resources::cpu, "cpu"},
      {logps::resources::ram, "ram"},
      {logps::resources::disk, "disk"}};
  int timeStamp;
  int secondsStopped;
  int cyclesRemaining;
  int quantumReceived;
  std::string resourceConsumed;
  std::string priority;
  std::string exportInfos() {
    std::stringstream ss;
    ss << "{";
    ss << R"("timeStamp":)" << timeStamp << ","
       << R"("secondsStopped":)" << secondsStopped << ","
       << R"("cyclesRemaining":)" << cyclesRemaining << ","
       << R"("quantumReceived":)" << quantumReceived << ","
       << R"("resourceConsumed":)" << '"' << resourceConsumed << '"' << ","
       << R"("priority":)" << '"' << priority << '"' << "}";
    return ss.str();
  }
  Logps(int timeStamp, int secondsStopped, int cyclesRemaining,
        int quantumReceived, int resourceConsumed, int priority)
      : timeStamp(timeStamp), secondsStopped(secondsStopped),
        cyclesRemaining(cyclesRemaining), quantumReceived(quantumReceived) {
    this->resourceConsumed =
        tResources.at(static_cast<logps::resources>(resourceConsumed));
    this->priority = tPriorities.at(static_cast<logps::priorities>(priority));
  }
};
class Log {

  logps::schedulerPolicy policy;
  std::map<int, std::vector<Logps>> mapPs;
  std::map<logps::schedulerPolicy, std::string> tPolicy = {
      {logps::schedulerPolicy::FIFO, "FIFO"},
      {logps::schedulerPolicy::LRU,  "LRU"},
      {logps::schedulerPolicy::MLQ,  "MLQ"}
      };

public:
  void addLog(int pid, Logps ps) {
    if (mapPs.find(pid) == mapPs.end()) {
      std::vector<Logps> qAux;
      qAux.push_back(ps);
      mapPs.insert({pid, qAux});
    } else {
      std::vector<Logps> qAux = mapPs.at(pid);
      qAux.push_back(ps);
      mapPs.at(pid) = qAux;
    }
  }
  void exportJson() const {
    std::stringstream ss;
    ss << "{";
    int commaPs{};
    int commaMap{};
    // printf("MAP SIZE:%lu", mapPs.size());
    for (auto pid : mapPs) {

      commaPs = 0;
      ss << '"' << pid.first << R"(":[)";
      for (auto logPs : pid.second) {
        ss << logPs.exportInfos();
        if (commaPs + 1 < pid.second.size()) {
          ss << ",";
          commaPs++;
        }
      }
      ss << "]";
      if (commaMap + 1 < mapPs.size()) {
        ss << ",";
        commaMap++;
      }
    }
    ss << "}";
    // printf("%s", ss.str().c_str());
    std::stringstream nameFile;
    nameFile << tPolicy.at(policy) << ".json";
    FileIO::createJson(nameFile.str(), ss.str());
  }
  // Log(std::string policy) {}
  Log(enum logps::schedulerPolicy policy) : policy(policy) {}
  Log() = default;
};
