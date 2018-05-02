#include <sstream>
#include <iostream>
#include <limits>
#include <cmath>
#include "scoreData.h"

ScoreData& ScoreData::getInstance() {
  static ScoreData score;
  return score;
}

ScoreData::~ScoreData( ) { }

ScoreData::ScoreData(const string& fn ) :
  scoreParser(fn),
  fileName( fn ),
  scoreData(scoreParser.getXmlData())
{ }

void ScoreData::setHighScore(int h) {
  std::stringstream strm;
  strm << h;
  scoreData["highScore"] = strm.str();
}


int ScoreData::getHighScore() const {
  std::string tag("highScore");
  std::map<std::string, string>::const_iterator ptr = scoreData.find(tag);
  if ( ptr == scoreData.end() )
    throw std::string("Game: Didn't find integer tag ")+tag+
          std::string(" in score xml");
  else {
    std::stringstream strm;
    strm << ptr->second;
    int data;
    strm >> data;
    return data;
  }
}

void ScoreData::writeTheXml() const {
  std::ofstream xmlFile;
  xmlFile.open (fileName);
  xmlFile << "<?xml version = \"1.0\"?>\n";
  xmlFile << "<GameData>\n";
  map<std::string, std::string>::const_iterator ptr =
    scoreData.begin();
  while ( ptr != scoreData.end() ) {
    xmlFile << "  " << "<" << ptr->first << ">";
    xmlFile << ptr->second;
    xmlFile << "</" << ptr->first << ">";
    xmlFile << std::endl;
    ++ptr;
  }
  xmlFile << "</GameData>\n";
  xmlFile.close();
}

void ScoreData::displayData() const {
  map<std::string, std::string>::const_iterator ptr =
    scoreData.begin();
  while ( ptr != scoreData.end() ) {
    std::cout << ptr->first << ", " << ptr->second << std::endl;
    ++ptr;
  }
}


