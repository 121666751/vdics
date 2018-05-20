/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#ifndef __DISTCONTROLLOR_H__
#define __DISTCONTROLLOR_H__

#ifdef __cplusplus
extern "C" {
#endif

struct DISTDATA;
struct DISTCTRLDATA;

/*
 * ���ݴ������ʼ���������������ݴ�����ָ�롣
 * @Handle ��[in] ���ƴ��ھ��
 * @����   �����ݴ�����ָ��
 */
HANDLE    __stdcall InitDistCtrl( HWND Handle );

/*
 * ���ݴ������ͷź����������ͷŷ�����ڴ�
 * @ hDistCtrl��[in] ���ݴ�����ָ�룬����InitDistCtrl�ķ���ֵ	
 */
void      __stdcall ReleaseDistCtrl( HANDLE Handle );
/*
 * ���������ͣ����������ʵ�����ݴ�����ͽ���ģ���ͬ����
 * @ hDistCtrl��[in] ���ݴ�����ָ�룬����InitDistCtrl�ķ���ֵ
 * @����    ��0-�ɹ� ����-ʧ��
 */
int       __stdcall PauseDist(HANDLE hDistCtrl);

/*
 * �������ֹͣ����������ʵ�����ݴ�����ͽ���ģ���ͬ����
 * @ hDistCtrl ��[in] ���ݴ�����ָ�룬����InitDistCtrl�ķ���ֵ
 * @����    ��0-�ɹ� ����-ʧ��	
 */
int       __stdcall StopDist(HANDLE hDistCtrl);
/*
 * ���ڴ洢�����ϸ���ݼ�״̬
 * @hDistCtrl��[in] ���ݴ�����ָ�룬����InitDistCtrl�ķ���ֵ
 * @����    ��0-�ɹ� ����-ʧ��
 */
int	   	  __stdcall StoreDistData(HANDLE hDistCtrl);

/*
 * ���ص�ǰ����Ʊ�ݵĿڴ���
 * @hDistCtrl��[in] ���ݴ�����ָ�룬����InitDistCtrl�ķ���ֵ
 * @����    ��0-�ɹ� ����-ʧ��	
 */
int       __stdcall GetPocketNo(HANDLE hDistCtrl);

/*
 * ���ص�ǰ����Ʊ�ݵ�����
 * @hDistCtrl��[in] ���ݴ�����ָ�룬����InitDistCtrl�ķ���ֵ
 * @����    ����ǰƱ�����ݽṹ��ָ��	
 */
DISTDATA* __stdcall GetOneData(HANDLE hDistCtrl);
/*
 * ����ǰ����ֿ���ģ���л�õ�Ʊ����Ϣ���뵽���ݴ���������������
 * @hDistCtrl��[in] ���ݴ�����ָ�룬����InitDistCtrl�ķ���ֵ
 * @pData   ��[in] Ʊ����Ϣ
 * @����    ��0-�ɹ� ����-ʧ��
 */
bool      __stdcall SetOneData( HANDLE hDistCtrl, DISTDATA *pData);
/*
 * ������ֽ衢�������ܶ�ܱ���
 * @hDistCtrl��[in] ���ݴ�����ָ�룬����InitDistCtrl�ķ���ֵ
 * @dramount��[out] �跽�ܽ��
 * @drcount  ��[out] �跽�ܱ���
 * @cramount��[out]  �����ܽ��
 * @crcount  ��[out] �����ܱ���
 * @����    ��0-�ɹ� ����-ʧ��
 */
int       __stdcall CalTotalAmount(HANDLE hDistCtrl, double* dramount, int* drcount, 
							 double* cramount, int* crcount);
/*
 * ��������ɾ��ָ����ŵ�����
 * @hDistCtrl  : [in] ���ݴ�����ָ�룬����InitDistCtrl�ķ���ֵ
 * @ightIndex  : [in] ���
 * @����    ��0-�ɹ� ����-ʧ��
 */
int       __stdcall DelSpecialItems(HANDLE hDistCtrl, int rightIndex);
/*
 * �������е���������
 * @hDistCtrl��[in] ���ݴ�����ָ�룬����InitDistCtrl�ķ���ֵ
 * @pData   ��[out] ������������
 * @count   ��[out] ����������������
 * @����    ��0-�ɹ� ����-ʧ��
 */
int       __stdcall GetAlltems(HANDLE hDistCtrl, DISTDATA* pData, int* count);

/*
 * ����ǰ����ֿ���ģ���л�õ�Ʊ����Ϣ���뵽���ݴ���������������
 * @hDistCtrl��[in] ���ݴ�����ָ�룬����InitDistCtrl�ķ���ֵ
 * @pData   ��[in] Ʊ����Ϣ
 * @����    ��0-�ɹ� ����-ʧ��
 */
int       __stdcall StartDstEx(HANDLE hDistCtrl,DISTCTRLDATA* pDcd);

#ifdef __cplusplus
}
#endif

#endif //__DISTCONTROLLOR_H__