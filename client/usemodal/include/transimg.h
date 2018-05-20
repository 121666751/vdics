#ifndef __TRANSIMG_H__
#define __TRANSIMG_H__

#include <libmem.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	*����ti_getfile()����ȡͼ���ļ�  
	*@param filename:		�ļ������������
	*@param ip:			ͼ��ַ������ip��ַ,�������
	*@param imgFileData:	ͼ���ļ�����,�������
	*@return 0---�ɹ�������---ʧ��
	*/
	int ti_getfile( const char* filename, const char* ip, MemT& imgFileData);

	/**
	*����ti_delfile()����ɾ��ͼ���ļ�  
	*@param filename:		�ļ������������
	*@param ip:			ͼ��ַ������ip��ַ,�������
	*@return 0---�ɹ�������---ʧ��
	*/
	int ti_delfile( const char* filename, const char* ip);

	/**
	*����ti_getfileformat()����ȡָ����ʽ���ļ���Ŀǰֻ���ڵ�ȡjpg�ļ�
	*@param filename:		�������ļ������������
	*@param format:		�ļ���ʽ���������
	*@param rate:			ѹ���ȣ��������
	*@param imgFileData:	ͼ���ļ����ݣ��������
	*@return 0---�ɹ�������---ʧ��
	*/
	int ti_getfileformat( const char* filename, const char* format, const char* ip, int rate, MemT& imgFileData);
	/**
	*����ti_filetoRGB()���ڽ�ͼ���ļ�����ת��ͨ��ͼ����
	*@param imgFileData:	ͼ���ļ�����,�������
	*@param format:		ͼ���ʽ��"bmp","jpg"�ȣ����������
	*@param pRGB:			ͨ��ͼ���ݣ��������
	*@param sizeX:			ͼ���ȣ��������
	*@param sizeY:			ͼ��߶ȣ��������
	*@param pannel:		ͨ�������������
	*@return 0---�ɹ�������---ʧ��
	*/
	int ti_filetoRGB(  MemT& imgFileData, const char* format, MemT& pRGB, int& sizeX, int& sizeY, int& pannel );
	/**
	*����ti_getfileformat_ex()����ȡָ����ʽ���ļ���Ŀǰֻ���ڵ�ȡjpg�ļ�
	*@param filename:		������bmp�ļ������������
	*@param format:		�ļ���ʽ���������
	*@param rate:			ѹ���ȣ��������
	*@param angel          ��ת�Ƕȣ��������
	*@param scale          ���ű������������
	*@param savename:	  ͼ���ļ����ݣ��������
	*@return 0---�ɹ�������---ʧ��
	*/
	int ti_getfileformat_ex( const char* filename, const char* format, const char* ip, int rate,int angel,double scale, const char * savename);

	/**
	*����ti_getfileformat_ex2()����ȡָ����ʽ���ļ���Ŀǰֻ���ڵ�ȡjpg�ļ�
	*@param filename:		������bmp�ļ������������
	*@param format:		�ļ���ʽ���������
	*@param rate:			ѹ���ȣ��������
	*@param angel          ��ת�Ƕȣ��������
	*@param scale          ���ű������������
	*@param imgFileData:	ͼ���ļ����ݣ��������
	*@return 0---�ɹ�������---ʧ��
	*/
	int ti_getfileformat_ex2( const char* filename, const char* format, const char* ip, int rate,int angel,double scale, MemT &imgData);
/**
 *����ti-rotateimg()����ͼ�����ݵ���ת
 *@param image:		    ͨ��ͼ���ݣ���������Ҳ�����
 *@param sizeX:			ͼ���ȣ���������Ҳ�����
 *@param sizeY:			ͼ��߶ȣ���������Ҳ�����
 *@param pannel:		ͨ�������������
 *@param angle:			��ת�Ƕȣ��������
 *@return 0---�ɹ�������---ʧ��
 */
int ti_rotateimg( MemT& image, int& sizeX, int& sizeY, int pannel, int angle);

/**
 *����ti_getimage()����������������������ȡͼ���ļ�������תͨ��ͼ����ת
 *@param filename:		�ļ������������
 *@param ip:			ͼ��ַ������ip��ַ,�������
 *@param angle			��ת�Ƕȣ��������
 *@param pRGB:			��ת���ͼ������,�������
 *@param sizeX:			ͼ����,�������
 *@param sizeY:			ͼ��߶�,�������
 *@param pannel:		ͨ����,�������
 *@return 0---�ɹ�������---ʧ��
 */
int ti_getimage( const char* filename, const char* ip, int angle, 
			    MemT& pRGB, int& sizeX, int& sizeY, int& pannel );
/**
 *����ti_getimagefast()���ڵ�ȡjpgͼ��������չ��Ϊbmp��ͼ����ļ�������imgsrv�˽�bmp�ļ�ת��ͬ����jpg�ļ������͹������ٽ���תͨ��ͼ����ת
 *@param filename:		�ļ���,�����ݿ�imgfile��ȡ�ã��������
 *@param ip:			ͼ��ַ������ip��ַ,�������
 *@param angle:			��ת�Ƕȣ��������
 *@param pRGB:			��ת���ͼ������,�������
 *@param sizeX:			ͼ����,�������
 *@param sizeY:			ͼ��߶�,�������
 *@param pannel:		ͨ����,�������
 *@return 0---�ɹ�������---ʧ��
 */
int ti_getimagefast(const char* filename, const char* ip, int angle,
				  MemT& pRGB, int& sizeX, int& sizeY, int& pannel);

unsigned char * ti_RGB2BMP(unsigned char *pRGB,long  width,long height,
			long baseX,long baseY,long left,long top,long right,long bottom,int &size);

int ti_RGB2BMPFREE(unsigned char *bmp);

#ifdef __cplusplus
}
#endif

#endif