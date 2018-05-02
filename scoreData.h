#ifndef SCOREDATA__H
#define SCOREDATA__H
#include <string>
#include <map>
#include "vector2f.h"
#include "parseXML.h"

class ScoreData {
public:
  static ScoreData& getInstance();
  ~ScoreData();
  void displayData() const;

  int  getHighScore() const;
  void setHighScore(int h);
  void writeTheXml() const;

private:
  ParseXML scoreParser;
  const std::string fileName;
  map<std::string, std::string> scoreData;

  ScoreData(const std::string& fn = "xmlSpec/score.xml");
  ScoreData(const ScoreData&);
  ScoreData& operator=(const ScoreData&);
};
#endif
