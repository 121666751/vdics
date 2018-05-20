/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#ifndef __TC_HDP_H__
#define __TC_HDP_H__

#ifdef __cplusplus
extern "C" {	
#endif
	/************************************************************************
	* �������ڳ�ʼ��������ֻ����õĶ�̬�⣬ͬʱ������������ģ�飬һ�����ڴ�����ֻ���õ����ݣ�һ�����ڴ�����������ֻ�������ָ�롣
	* @hwnd �� [in] ���ƴ��ھ��
	* @proc �� [in] ���ݴ���ģ�飬���ڷ�����õ�micr,ocr�ȵ�
	* @perr �� [in] ���������Ϣ
	* @���� �� ��ֻ�������ָ��                                                                     
	/************************************************************************/
	HANDLE 
		__stdcall InitMDL(HWND hwnd,void (*post_proc)(int num),void (*post_err)(int condition));		//Initialize TCHdpMDL and return its instance handle
	/*
	*	���������ͷ���ֻ�����ʵ�����ͷ��Ѽ��صĶ�̬���
	*  @handle ����ֻ�������ָ�룬����InitMDL ��ʼ�������ķ���ֵ
	*/
	void
		__stdcall FreeMDL(HANDLE handle);							//Release the TCHdpMDL instance
	/*
	* ��ֻ���ʼ�����������ڴ�com��,��ʼ��ocrʶ������
	* @handle ��[in]��ֻ�������ָ�룬����InitMDL ��ʼ�������ķ���ֵ
	* @����   ��0-�ɹ� ����-ʧ��
	*/
	int
		__stdcall HDPOpen(HANDLE handle);							//Connect with HDP sorter
	/*
	* ��ֻ��رջ����������ڹر�com��,�ر�ocrʶ������
	* @handle ��[in]��ֻ�������ָ�룬����InitMDL ��ʼ�������ķ���ֵ
	* @����   ��0-�ɹ� ����-ʧ��
	*/
	int
		__stdcall HDPClose(HANDLE handle);							//Disconnect with HDP sorter
	/*
	* ��ֻ����к���������������ֻ�������Ʊ
	* @handle   ��[in]��ֻ�������ָ�룬����InitMDL ��ʼ�������ķ���ֵ
	* @mode     ��[in]���ģʽ  2-256�Ҷ�ģʽ  3-24���ɫģʽ
	* @initflag ��[in]�Ƿ���Ҫ���ڲ��������г�ʼ������ true-��Ҫ false-����Ҫ
	* @����     ��0-�ɹ� ����-ʧ��
	*/
	int
		__stdcall HDPStart(HANDLE handle,int mode, int speed, bool initflag); 				//Start Sort with HDP sorter

	int
		__stdcall HDPStop(HANDLE handle);							//Stop the HDP sorter
	/*
	* �������ڻ�ȡ��Ҫ�洢��ͼ������
	* @handle ��[in]��ֻ�������ָ�룬����InitMDL ��ʼ�������ķ���ֵ
	* @fname  ��[in]��Ҫ�����ͼ���ļ���
	* @����   ��0-�ɹ� ����-ʧ��
	*/
	int
		__stdcall HDPGetImg(HANDLE handle, char *fname);			//Get voucher image from the HDP sorter
	/*
	* ��������Ӳ����ȡƱ�ݴ�����Ϣ
	* @handle ��[in]��ֻ�������ָ�룬����InitMDL ��ʼ�������ķ���ֵ
	* @num    ��[in] Ʊ��������
	* @micr   ��[out] Ʊ�ݴ�����Ϣ
	* @����   ��0-�ɹ� ����-ʧ��
	*/
	int
		__stdcall HDPGetMicr(HANDLE handle, int num, char *micr);	//Get MICR code from the HDP sorter
	/*
	* �������������ȡƱ�ݴ�����Ϣ
	* @handle ��[in]��ֻ�������ָ�룬����InitMDL ��ʼ�������ķ���ֵ
	* @num    ��[in] Ʊ��������
	* @ocr    ��[out] Ʊ�ݴ�����Ϣ
	* @����   ��0-�ɹ� ����-ʧ��
	*/


	int	__stdcall HDPGetOcr(HANDLE handle, int num, char *ocr);

	/*
	* �������������ȡƱ�ݴ�����Ϣ
	* @milSys ��[in]milָ��
	* @fname  ��[in] ͼ����
	* @ocr    ��[out] Ʊ�ݴ�����Ϣ
	* @����   ��0-�ɹ� ����-ʧ��
	*/
int
 NewHDPGetOcr(char *fname,char*ocr);
/*
 * �����������ÿڴ�����Ʊ����ֵ�ָ���ڴ�
 * @handle ��[in]��ֻ�������ָ�룬����InitMDL ��ʼ�������ķ���ֵ
 * @pocket ��[in]�ڴ���
 * @����   ��0-�ɹ� ����-ʧ��
 */



int
__stdcall HDPSetPocket(HANDLE handle, int pocket);			//Set the sort pocket of the HDP sorter
/*
 * �����������е�Ʊ��
 * @handle ��[in]��ֻ�������ָ�룬����InitMDL ��ʼ�������ķ���ֵ
 * @����   ��0-�ɹ� ����-ʧ��
 */
int
__stdcall HDPPurge(HANDLE handle);							//Clear the current voucher in the HDP sorter
/*
 * ��������������ֵĻ�ɫ
 * @handle ��[in]��ֻ�������ָ�룬����InitMDL ��ʼ�������ķ���ֵ
 * @����   ��0-�ɹ� ����-ʧ��
 */
int
__stdcall HDPBaseSet(HANDLE handle);						//Set the base color of the HDP sorter
/*
 * ������ֻ���ֲ�������ͼ��Χ��ɨ�跽ʽ��
 * @handle ��[in]��ֻ�������ָ�룬����InitMDL ��ʼ�������ķ���ֵ
 * @mode   ��[in] ɫ��ģʽ 2-256���Ҷ� 3-��ɫ
 * @����   ��0-�ɹ� ����-ʧ��	
 */
int
__stdcall HDPAdSet(HANDLE handle, int mode, int speed);		//Set the speed and wide of the HDP sorter
/*
 * ʵ�ֱ����ӡ����
 * @handle ��[in]��ֻ�������ָ�룬����InitMDL ��ʼ�������ķ���ֵ
 * @str    ��[in] ��Ҫ��ӡ������
 * @on     ��[in] ��ʼλ��
 */
int
__stdcall HDPEndorse(HANDLE handle, char *str, int on);
/*
 * @��Ϣ�����������������ⲿ����
 * @errcode :������
 */
void
__stdcall HDPErrMsg(int errcode);	//Return the error explaination of the HDP error code

int 
__stdcall HDPdecBankPtr(HANDLE handle);
/*
 * �������ڽ��ɼ�����ͼ�����ݴ浽Ӳ���ϡ�
 * @handle ��[in]��ֻ�������ָ�룬����InitMDL ��ʼ�������ķ���ֵ
 * @index  ��[in]Ʊ��������
 * @fname  ��[in]��Ҫ���������ͼ���ļ���
 * @rname  ��[in]��Ҫ����ķ���ͼ���ļ���
 * @����   ��0-�ɹ� ����-ʧ��
 */
int 
__stdcall HDPSaveImage(HANDLE handle,int index,char *fname,char *rname);

#ifdef __cplusplus
}
#endif

#endif //__TC_HDP_H__