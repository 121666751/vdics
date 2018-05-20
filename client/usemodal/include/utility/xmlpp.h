#ifndef __LIBXML_HPP__
#define __LIBXML_HPP__

#include <string>
#include <iconv.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>


namespace xml {
/////////////////////////////////////////////////////////////////////////////

class NodeT;


/**
 * class GbEncodingT
 * ���ڽ�UTF-8������ַ���ת��ΪGB��ʽ
 */
class GbEncodingT
{
public:
	/**
	 * @param utfStr: UTF-8������ַ���
	 * @return: GB18030������ַ���
	 */
	std::string toGb(std::string utfStr);

	std::string operator()(std::string utfStr) { return toGb(utfStr); }

	GbEncodingT() { _cd = iconv_open("GB18030","UTF-8"); }
	virtual ~GbEncodingT() { iconv_close(_cd); }

protected:
	iconv_t _cd;
};




/**
 * class DocT
 * ����xml�ı�
 */
class DocT {

friend NodeT;

public:

	/**
	 * @param fileName: xml�ı��ļ���
	 * @return 0-success
	 */
	int parseFile(std::string fileName);

	/**
	 * @param str: xml�ַ���
	 * @return 0-success
	 */
	int parseMemory(std::string& str);

	/**
	 * ȡ��root�ڵ�
	 * @return: root�ڵ����NodeT
	 */
	NodeT getRoot(); 

	DocT()	{	_theDoc = NULL;	}
	virtual ~DocT() {  release_underlying();	}

protected:
	void release_underlying() {	if( _theDoc ) { xmlFreeDoc(_theDoc);_theDoc = NULL;	}	}

	// member varibles
	xmlDocPtr _theDoc;
	GbEncodingT _gb;
};


/** 
 * class NodeT
 * ����xml�ڵ�
 */
class NodeT {
public:

	/**
	 * @return: ��ǰ�ڵ�����	
	 */
	std::string name(); 

	/**
	 * @param doc: ���ڴ����DocT�Ķ���
	 * @return: ��ǰ�ڵ�����
	 */
	std::string value();

	/**
	 * @param name: ��������
	 * @return: ����ֵ
	 */
	std::string prop(std::string name);

	/**
	 * @return: ��ǰxml�ڵ���ַ����������ڵ�	
	 */
	std::string content();

	/**
	 * ���뵽�ӽڵ�	
	 */
	void child() { if(_theNode) _theNode=_theNode->xmlChildrenNode; 
					_firstNodeFetched=false; }

	/**
	 * ���뵽��һ���ڵ�	
	 */
	void next()	{ if(_theNode) _theNode=_theNode->next; }

	/**
	 * @return: true-��ǰ�ڵ�Ϊ��, false-��ǰ�ڵ���Ч	
	 */
	bool empty() { return (_theNode==NULL); }

	/**
	 * �״ε���״̬Ϊ��ǰ�ڵ㣬�ٴε�����η���������һ���ڵ�
	 * @return: true-ȡ�õĽڵ���Ч	
	 */
	bool fetch(); 

	/**
	 * ����libxml2�е�xmlFree()������c++builder�л������ڴ���ʴ��� �����ó�Ա���������
	 */
	void xmlFree(xmlChar* key);

	NodeT(NodeT& no):_theNode(no._theNode),_doc(no._doc),_gb(no._gb),_firstNodeFetched(no._firstNodeFetched) {;}
	NodeT(xmlNodePtr pNode, DocT& doc):_theNode(pNode),_doc(doc),_gb(doc._gb),_firstNodeFetched(false) {;}

protected:
	xmlNodePtr _theNode;
	DocT& _doc;
	GbEncodingT& _gb;
	bool _firstNodeFetched;		// ��־��ǰ�ڵ��Ƿ��ѱ�fetch
};


/////////////////////////////////////////////////////////////////////////////////
// DocT inline section
inline int DocT::parseFile(std::string fileName)
{
	release_underlying();	// release orgin alloced theDoc handle
	if(fileName.empty()) return -1;
	
	_theDoc = xmlParseFile(fileName.c_str()); 
	if( !_theDoc ) return -1;
	return 0;
};

inline int DocT::parseMemory(std::string& str)
{
	release_underlying();	// release orgin alloced theDoc handle
	if(str.empty()) return -1;

	
	_theDoc = xmlParseMemory(str.c_str(),str.length()); 
	if( !_theDoc ) return -1;
	return 0;

}

inline NodeT DocT::getRoot()
{ 
	return NodeT( xmlDocGetRootElement(_theDoc),*this); 
}


///////////////////////////////////////////////////////////////////////////////////
// NodeT inline section

inline std::string NodeT::name()
{ 
	return _gb((char*)_theNode->name); 
}

inline std::string NodeT::value()
{
	if(!_theNode) return "";
	std::string val;
	xmlChar* key = xmlNodeListGetString(_doc._theDoc, _theNode->xmlChildrenNode, 1);
	if(key) {
		val = (char*)key;
		xmlFree(key);
	}

	return _gb(val);
}

inline std::string NodeT::prop(std::string name)
{
	if(!_theNode) return "";
	std::string val;
	xmlChar* key = xmlGetProp(_theNode, (xmlChar*)name.c_str());
	if(key) {
		val = (char*)key;
		xmlFree(key);
	}

	return _gb(val);
}

inline std::string NodeT::content()
{
	if(!_theNode) return "";
	std::string val;
	xmlChar* key = xmlNodeGetContent(_theNode);
	if(key) {
		val = (char*)key;
		xmlFree(key);
	}

	return _gb(val);
}

inline bool NodeT::fetch()
{
	if( _firstNodeFetched ) next();
	else _firstNodeFetched = true;

	return (_theNode!=NULL);
}

inline void NodeT::xmlFree(xmlChar* key)
{
#ifndef __BORLANDC__
	::xmlFree(key);
#endif
}

///////////////////////////////////////////////////////////////////////////////////
// Encoding inline section
inline std::string GbEncodingT::toGb(std::string utfStr)
{
	if( utfStr.empty() ) return "";

	unsigned int inlen = utfStr.length();
	unsigned int outlen = inlen*2+10;
	char* outbuf = new char[outlen];
	memset(outbuf,0,sizeof(outlen));

	const char* inbuf = utfStr.c_str();
	char* o = outbuf;
	size_t convlen = iconv(_cd, &inbuf, &inlen, &o, &outlen );

	std::string gbstr(outbuf,o-outbuf);
	delete[] outbuf;
	return gbstr;
}
///////////////////////////////////////////////////////////////////////////////////

};

//////////////////////////////////////////////////////////////////
// Auto Link Section
#if defined(_MSC_VER) && !defined(__BORLANDC__)
	#pragma comment(lib,"libxml2.lib")
	#pragma comment(lib,"iconv.lib")
#endif

#endif //__LIBXML_HPP__