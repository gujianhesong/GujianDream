#include "CsvUtil.h"
#include "cocos2d.h"


CsvUtil* CsvUtil::_gInstance;

CsvUtil::CsvUtil():_pCsvMap(nullptr)
{
    _pCsvMap = new CsvMap();
}

CsvUtil::~CsvUtil()
{
    CC_SAFE_DELETE(_pCsvMap);
}

CsvUtil* CsvUtil::getInstance()
{
    if (!_gInstance)
        _gInstance = new CsvUtil();
    return _gInstance;
}

void CsvUtil::destroyInstance()
{
    if(_gInstance) CC_SAFE_DELETE(_gInstance);
}

void CsvUtil::addFileData(const std::string &rSCsvFilePath)
{

    if(std::string("") == rSCsvFilePath) return;

    auto linesVec = getDataLines(rSCsvFilePath);
    StrVec strsVec;
    StrDict dict;
    
    for(const auto &linesVecIter : linesVec)
    {
        strsVec = split(linesVecIter.c_str(), ",");
        dict.push_back(strsVec);
    }
    _pCsvMap->insert(std::make_pair(std::string(rSCsvFilePath), dict));
}

void CsvUtil::releaseFile(const std::string &rSCsvFilePath)
{
    _pCsvMap->erase(rSCsvFilePath);
}

std::tuple<int, int> CsvUtil::getFileRowColNum(const std::string &rSCsvFilePath)
{
    auto tDict = getFileDict(rSCsvFilePath);
    int iRowNum = tDict.size();
    int iColNum = (*(tDict.begin())).size();
    return std::make_tuple(iColNum, iRowNum);
}

std::string CsvUtil::getText(const int &rRow, const int &rCol, const std::string &rSCsvFilePath)
{
    const auto dict = getFileDict(rSCsvFilePath);
    
    const auto lineDatas = dict.at(rRow);
    
    return lineDatas.at(rCol);
}

int CsvUtil::getInt(const int &rRow, const int &rCol, const std::string &rSCsvFilePath)
{
    return atoi(getText(rRow, rCol, rSCsvFilePath).c_str());
}

double CsvUtil::getDouble(const int &rRow, const int &rCol, const std::string &rSCsvFilePath)
{
    return atof(getText(rRow, rCol, rSCsvFilePath).c_str());
}

StrVec CsvUtil::getRowData(const int &rIRow, const std::string &rSCsvFilePath)
{
    auto tRow = std::get<1>(getFileRowColNum(rSCsvFilePath));
    
    if(rIRow > tRow) return StrVec();
    
    return  _pCsvMap->at(rSCsvFilePath).at(rIRow);
}

StrDict &CsvUtil::getFileDict(const std::string &rSCsvFilePath)
{
    return _pCsvMap->at(rSCsvFilePath);
}

int CsvUtil::findValueInWithLine(const std::string &rSValue, const int &rIValueCol, const std::string &rSCsvFilePath)
{
    auto iRowCount = std::get<1>(getFileRowColNum(rSCsvFilePath));
    
    auto ret = -1;
    std::string findValue(rSValue);
    
    for (int iRow = 0; iRow < iRowCount; ++iRow)
    {
        std::string tmpValue = getText(iRow, rIValueCol, rSCsvFilePath);
		
        if (findValue.compare(tmpValue) == 0)
        {
            ret = iRow;
            break;
        }
    }
    return ret;
}

