#ifndef __gujian__GFileUtils__
#define __gujian__GFileUtils__

#include "cocos2d.h"
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
	/* ���ļ���ȡ����                                                                     */
	/************************************************************************/
	static unsigned char* readFile(string pFileName, ssize_t &len){      
		//��¼cocos2d-x��CCFileUtils������û���ҵ��ļ��Ƿ񵯳���ʾ�������    
		bool isNeedModifyPopupSetting  = FileUtils::sharedFileUtils()->isPopupNotify();    
		//�������ʾ������ʱ�رգ���Ϊ����Ķ�ȡ�����Ҳ������ļ�����Ϊ���ļ��п��ܻ�û�д���    
		if(isNeedModifyPopupSetting)    
		{    
			CCFileUtils::sharedFileUtils()->setPopupNotify(false);    
		}     
		//��ȡ�ļ���·��,ʹ��getWritablePath����Ϊ����ļ���������Ҫ�洢���ļ�    
		string path = FileUtils::sharedFileUtils()->getWritablePath() + pFileName;     
		CCLog("path = %s",path.c_str());    
		//��ȡ�ļ���ע��ʹ�ò���"rb"��r��ʾread��b��ʾ������binary    
		unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rb", &len);    
		CCLog("read data length = %d", len);    
		//�����ǰ�����Ҳ����ļ�����ʾ����Ļ�ԭ��������    
		if(isNeedModifyPopupSetting)    
		{    
			CCFileUtils::sharedFileUtils()->setPopupNotify(true);    
		}    
		return data;      
	}  


	/************************************************************************/
	/* �������ݵ��ļ�                                                                     */
	/************************************************************************/
	static bool saveFile(unsigned char *pContent, string pFileName, int length){      
		//��ȡ������ļ�·��     
		string path = FileUtils::sharedFileUtils()->getWritablePath() + pFileName;      
		CCLog("save file path = %s",path.c_str());      

		//����һ���ļ�ָ�룬ע��Ҫʹ�ò���"wb"��w��ʾwrite��b��ʾ������binary��֮ǰ��ʹ�õ���"w",ios�ϵ�ʱû�з������⣬����win32�ϻ���bug���ĳ�"wb"��û��������    
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