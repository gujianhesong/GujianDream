#ifndef __gujian__GFileUtils__
#define __gujian__GFileUtils__

#include "cocos2d.h"
#include "ByteBuffer.h"

using namespace std;
USING_NS_CC;

class GFileUtils
{
public:
	GFileUtils::GFileUtils(){}

	GFileUtils::~GFileUtils(){}

public:

	static bool saveBuffer(string pFileName, ByteBuffer& buffer){
		return GFileUtils::saveFile((unsigned char*)buffer.contents(), pFileName, buffer.size());
	} 

	static ByteBuffer readBuffer(string pFileName){
		ssize_t len = 0;
		unsigned char* readData = GFileUtils::readFile(pFileName, len);
		ByteBuffer buffer;
		buffer.append(readData, len);

		return buffer;
	}

	/************************************************************************/
	/* 从文件读取内容                                                                     */
	/************************************************************************/
	static unsigned char* readFile(string pFileName, ssize_t &len){      
		//记录cocos2d-x中CCFileUtils，对于没有找到文件是否弹出提示框的设置    
		bool isNeedModifyPopupSetting  = FileUtils::sharedFileUtils()->isPopupNotify();    
		//如果有提示，就暂时关闭，因为这里的读取可能找不到该文件，因为该文件有可能还没有创建    
		if(isNeedModifyPopupSetting)    
		{    
			CCFileUtils::sharedFileUtils()->setPopupNotify(false);    
		}     
		//获取文件的路径,使用getWritablePath是因为这个文件是我们需要存储的文件    
		string path = FileUtils::sharedFileUtils()->getWritablePath() + pFileName;     
		CCLog("path = %s",path.c_str());    
		//读取文件，注意使用参数"rb"，r表示read，b表示二进制binary    
		unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rb", &len);    
		CCLog("read data length = %d", len);    
		//如果以前设置找不到文件有提示，则改回原来的设置    
		if(isNeedModifyPopupSetting)    
		{    
			CCFileUtils::sharedFileUtils()->setPopupNotify(true);    
		}    
		return data;      
	}  


	/************************************************************************/
	/* 保存内容到文件                                                                     */
	/************************************************************************/
	static bool saveFile(unsigned char *pContent, string pFileName, int length){      
		//获取储存的文件路径     
		string path = FileUtils::sharedFileUtils()->getWritablePath() + pFileName;      
		CCLog("save file path = %s",path.c_str());      

		//创建一个文件指针，注意要使用参数"wb"，w表示write，b表示二进制binary，之前我使用的是"w",ios上当时没有发现问题，但是win32上会有bug，改成"wb"就没有问题了    
		FILE* file = fopen(path.c_str(), "wb");     

		if (file) {     
			fwrite(pContent, sizeof(unsigned char), length, file);    
			fclose(file);      
		}      
		else    
		{    
			CCLog("save file error.");      
		}      
		return false;      
	}  


};

#endif 