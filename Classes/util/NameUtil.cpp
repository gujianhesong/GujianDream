#include "NameUtil.h"
#include <stdlib.h>
#include "GFileUtils.h"
#include "basic_types.h"
#include "PublicDefine.h"
#include <string>

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

using namespace std;
using namespace rapidjson;



std::string NameUtil::createName()
{
	
	rapidjson::Document m_doc;
	std::string jsonpath = FileUtils::getInstance()->fullPathForFilename(NAMES_FILE.c_str());
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
    m_doc.Parse<0>(contentStr.c_str());
    printf("%s\n",contentStr.c_str());
    
    if (m_doc.HasParseError()) {
        CCAssert(false, "Json::skill_info.json Reader Parse error!");
    }

	string name = "";
    
    if (!m_doc.IsNull()){  //判断是不是数组
		rapidjson::Value & nameFirst = m_doc["name_first"];
		if(nameFirst.IsArray() && nameFirst.Size() > 0){
			int index = rand() % nameFirst.Size();
			rapidjson::Value& value = nameFirst[index];
			name.append(value.GetString());
		}

        rapidjson::Value & nameSecond = m_doc["name_second"];
		if(nameSecond.IsArray() && nameSecond.Size() > 0){
			int index = rand() % nameSecond.Size();
			rapidjson::Value& value = nameSecond[index];
			name.append(value.GetString());
		}
    }

	return name;
}

NameUtil::NameUtil()
{

}

NameUtil::~NameUtil()
{
	
}


