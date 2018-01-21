#ifndef __NameUtil__
#define __NameUtil__

#include <string>

class NameUtil
{
public:
	NameUtil();

	virtual ~NameUtil();

	static std::string createName();
protected:

	// 3 个中文占 6 个字符
	char m_szName[7];
	

};


#endif