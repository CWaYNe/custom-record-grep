#ifndef CRGREP_FORMATTER_H
#define CRGREP_FORMATTER_H

#include <deque>

std::tuple<std::string, int> getTagTuple(std::string tagType, long tagLocation);

// one file -> one record
// one dir -> merge every file in that dir into one record

class Formatter{
public:
  Formatter(std::string pathSource, std::string pathDest);
  Formatter(std::string pathSource, std::string pathDest, std::string pathStopWords);
  ~Formatter(){};
  
private:
  std::string pathToFormattedDir;
  std::string pathToRawData;
  std::string pathToStopWords;

  std::vector <std::string> stopWords;
  std::vector <std::string> singleFilePaths;
  std::vector <std::string> concatFilePaths;

  void readPathsInDirAndProcessEachPath();
  void processFile(bool concatFlag=false);
  void newProcessFile(std::string pathToChildDir, bool concatFlag=false);
  void processConcatFile(std::string pathToDir,
                         long &char_count,
                         int &chapter_num,
                         int &title_num,
                         int &paragraph_num,
                         int &sentense_num,
                         std::string dataTitle);

  void writeRecordHeaderToFile(std::string dataTitle);

  void formatThenMerge(std::string pathToSingleFile,
                       long &char_count,
                       int &chapter_num,
                       int &title_num,
                       int &paragraph_num,
                       int &sentense_num,
                       std::string dataTitle);


  void writeTagInfoToFile(std::deque <std::tuple <std::string, long>> &tagQueue, std::string dataTitle);

  void lineFormatter(std::string &line,
                     std::string &regexEndingPhrasePattern,
                     int &sentense_num,
                     long &char_count,
                     std::deque <std::tuple <std::string, long>> &tagQueue);

  void insertFilesPathInDirIntoVector(std::string path, std::vector <std::string> &paths);

  void insertDirPathInRawDir();

};



#endif