#ifndef __gujian__CsvUtil__
#define __gujian__CsvUtil__

#include <vector>
#include <map>
#include <string>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

typedef std::vector<std::string> StrVec;

typedef std::vector< StrVec > StrDict;

typedef std::map< std::string, StrDict> CsvMap;

class CsvUtil
{

public:
    
    static CsvUtil* getInstance();
    
    static void destroyInstance();
    
    /**
     *  add csv file to dict
     *
     *  @param sPath is csv file path
     */
    virtual void addFileData(const std::string &rSPath);
    
    /**
     *  del csv file to dict
     *
     *  @param sPath is csv file path
     */
    virtual void releaseFile(const std::string &rSPath);
    

    /**
     *  get some row and some column value from some csv file
     *
     *  @param rRow        is row num
     *  @param rCol        is column num
     *  @param csvFilePath is some csv file
     *
     *  @return some row and some column real TextValue IntValue, DoubleValue,if this pos not be exsit return ""
     */
    virtual std::string getText(const int &rRow, const int &rCol, const std::string &rSCsvFilePath);
    virtual int getInt(const int &rRow, const int &rCol, const std::string &rSCsvFilePath);
    virtual double getDouble(const int &rRow, const int &rCol, const std::string &rSCsvFilePath);
    
    
    /**
     *  get some file row data
     *
     *  @param iRow          is row num
     *  @param rSCsvFilePath is some csv file
     *
     *  @return some row data
     */
    virtual StrVec getRowData(const int &rIRow, const std::string &rSCsvFilePath);
    
    /**
     *  get csv file row and column save tuple<int, int>
     *
     *  @param rSCsvFilePath csv file path
     *
     *  @return csv file row, column in tuple<int, int>
     */
    virtual std::tuple<int, int> getFileRowColNum(const std::string &rSCsvFilePath);
    
    /**
     *  find dest value in csv file row num
     *
     *  @param rSValue       find value
     *  @param rIValueCol    value column
     *  @param rSCsvFilePath csv file path
     *
     *  @return value in csv row
     */
    virtual int findValueInWithLine(const std::string &rSValue, const int &rIValueCol, const std::string &rSCsvFilePath);

	/**
     *  get csv file string vec
     *
     *  @param rSCsvFilePath csv file path
     *
     *  @return csv file strVec
     */
    virtual StrDict &getFileDict(const std::string &rSCsvFilePath);


protected:
    
    virtual ~CsvUtil();
    
public:
    
    CsvUtil();
    
    /*CsvUtil(const CsvUtil &rCsvUtil) = delete;
    
    CsvUtil &operator=(const CsvUtil &rCsvUtil) = delete;*/
    
private:
    
    static CsvUtil* _gInstance;
    
    /**
     * 
     */
    CsvMap* _pCsvMap;

private:

	StrVec getDataLines(const std::string &rSCsvFilePath)
    {
        StrVec linesList;

		CCLOG("==============%s", rSCsvFilePath.c_str());
        if (!FileUtils::getInstance()->isFileExist(rSCsvFilePath))
            return linesList;
        
        ssize_t pSize = 0;
		CCLOG("==============%s", rSCsvFilePath.c_str());
		string fullpath = FileUtils::getInstance()->fullPathForFilename(rSCsvFilePath); 
        unsigned char* data = FileUtils::getInstance()->getFileData(rSCsvFilePath, "r", &pSize);
        CCLOG("==============%s", data);
        if(data) linesList = split((char *)data, "\n");

		linesList.erase(linesList.begin());
        
        return linesList;
    }

	StrVec split(const std::string &rSSrcStr, const std::string &rSSep)
    {
        StrVec stringList;
        
        int size = rSSrcStr.size();
        
        std::string str(rSSrcStr);
        
        int startIndex = 0;
        
        int endIndex = 0;
        
        endIndex = str.find(rSSep);
        
        std::string splitStr = "";
        
        while (endIndex > 0)
        {
            splitStr = str.substr(startIndex, endIndex);
            
            stringList.push_back(splitStr);
            
            str = std::string(str.substr(endIndex + 1, size));
            
            endIndex = str.find(rSSep);
        }
        
        if (str.compare("") != 0) {
            stringList.push_back(str);
        }
        return stringList;
    }

};


#endif // !__CsvUtil__