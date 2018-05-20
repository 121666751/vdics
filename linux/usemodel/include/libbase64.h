/**
 * @file
 *
 * ���ݱ��롢���롢ѭ��У�麯����
 */

#ifndef __LIBBASE64_H__ 
#define __LIBBASE64_H__

#ifdef __cplusplus
	extern "C"{
#endif


/**
 * ˵������Դ���ݰ�BASE64���봫��
 * ע�⣺��Ҫ�ڵ��ô˺���֮ǰ���ٳ����Ŀ�����ݵ��ڴ�,�ڴ��СΪԴ���ݳ��ȵ�2��,�ڴ�ʹ�ú��ɵ����������ͷ�.
 * 
 * @param src: Դ����
 * @param srclen: Դ���ݳ���
 * @param dest:������Ŀ������
 * @param msize:Ŀ�����ݵĳ���
 * @return 0-�ɹ�, other-ʧ��.
 */
int encode_base64(void *src,int srclen,char *dest,int *msize);


/**
 * ˵��������BASE64�����Դ���ݽ���
 * ע�⣺��Ҫ�ڵ��ô˺���֮ǰ���ٳ����Ŀ�����ݵ��ڴ�,�ڴ��СΪԴ���ݳ��ȵ�1��,�ڴ�ʹ�ú��ɵ����������ͷ�.
 * 
 * @param src: �����Դ���ݴ�
 * @param srclen:�����Դ���ݴ�����
 * @param dest:������Ŀ������
 * @param msize:������Ŀ�����ݵĳ���
 * @return 0-�ɹ�, other-ʧ��.
 */
int decode_base64(char *src,int srclen,void *dest,int *msize);


/**
 * ˵������Դ���ݰ�yencode���봫��
 * ע�⣺��Ҫ�ڵ��ô˺���֮ǰ���ٳ����Ŀ�����ݵ��ڴ�,�ڴ��СΪԴ���ݳ��ȵ�2��,�ڴ�ʹ�ú��ɵ����������ͷ�.
 * 
 * @param src: Դ����
 * @param srclen: Դ���ݳ���
 * @param dest:������Ŀ������
 * @param msize:Ŀ�����ݵĳ���
 * @return 0-�ɹ�, other-ʧ��.
 */
int yencode(void *src,int srclen,char **dest,int *msize);


/**
 * ˵��������yencode�����Դ���ݽ���
 * ע�⣺��Ҫ�ڵ��ô˺���֮ǰ���ٳ����Ŀ�����ݵ��ڴ�,�ڴ��СΪԴ���ݳ��ȵ�1��,�ڴ�ʹ�ú��ɵ����������ͷ�.
 * 
 * @param src: �����Դ���ݴ�
 * @param srclen:�����Դ���ݴ�����
 * @param dest:������Ŀ������
 * @param msize:������Ŀ�����ݵĳ���
 * @return 0-�ɹ�, other-ʧ��.
 */
int ydecode(char *src,int srclen,void **dest,int *msize);


/**
 * ˵��������Դ���ݵ�ѭ��У��ֵ

 * @param src: Դ����
 * @param srclen: Դ���ݳ���
 * @param dst:ѭ��У��ֵ
 * @return 0-�ɹ�, other-ʧ��.
 */
int CRC32_Digest(void *src,int srclen,unsigned int *dst);


/**
 * ˵������Դ���ݰ�һ�ֱ���ת��Ϊ��һ�ֱ���
 * ע�⣺��Ҫ�ڵ��ô˺���֮ǰ���ٳ����Ŀ�����ݵ��ڴ�,�ڴ��С�ӱ��벻ͬ��������ͬ,�ڴ�ʹ�ú��ɵ����������ͷ�.
 * 
 * @param inbuf: Դ����
 * @param inlen: Դ���ݳ���
 * @param outbuf:Ŀ������
 * @param outlen:Ŀ�����ݵĳ���
 * @param fromcode:Դ���ݱ���
 * @param tocode:��Ҫת�������ݱ���
 * @return 0-�ɹ�, other-ʧ��.
 */
int iconvMem(char *inbuf,int *inlen,char *outbuf,int *outlen,char *fromcode,char *tocode);


/**
 * ˵������Դ���ݰ�yencode����,
 		 ����=ybegin filename=xxx size=xxx ��ʼ, ����=yend filename=xxx size=xxx CRC32=xxx����,�õ�Ŀ������
		 ����Myencode����ֱ�Ӷ�ԭʼ���ݽ���yencode����,Myencode_ex�������Ƚ�ԭʼ���ݽ���zlibѹ��,Ȼ���ڶ�ѹ��
		 ���ݱ���.

 * ע�⣺��Ҫ�ڵ��ô˺���֮ǰ���ٳ����Ŀ�����ݵ��ڴ�,�ڴ��СΪԴ���ݳ��ȵ�2��+100,�ڴ�ʹ�ú��ɵ����������ͷ�.
 * 
 * @param src: Դ����
 * @param srclen: Դ���ݳ���
 * @param filename:���ݱ�ʶ�ؼ���
 * @param dest:������Ŀ������(��ǰ׺����׺)
 * @param msize:Ŀ�����ݵĳ���
 * @return 0-�ɹ�, other-ʧ��.
 */
int Myencode(void *src,int srclen,char *filename,char *dst,int *dstlen);
int Myencode_ex(void *src,int srclen,char *filename,char *dst,int *dstlen);

/**
 * ˵��: �õ��������ݳ���
 * @param src: ������Դ����
 * @return >0: �õ��������ݳ���, -1: Դ���ݴ���
 */
int MyGetDecodeSize(char* src);

/**
 * ˵��������yencode����,����=ybegin filename=xxx size=xxx ��ʼ,��=yend filename=xxx size=xxx CRC32=xxx������Դ����,
 		 ����õ�Ŀ������
 * ע�⣺��Ҫ�ڵ��ô˺���֮ǰ���ٳ����Ŀ�����ݵ��ڴ�,�ڴ��С��ͨ��MyGetDecodeSize�õ�,
		 �ڴ�ʹ�ú��ɵ����������ͷ�.
 * 
 * @param src: Դ����(��ǰ׺����׺)
 * @param filename:���ݱ�ʶ�ؼ���
 * @param dst:������Ŀ������
 * @param dstlen:Ŀ�����ݵĳ���
 * @return 0-�ɹ�, other-ʧ��.
 */
int Mydecode(char *src,char *filename,void *dst,int *dstlen);

#ifdef __cplusplus
}

#endif

#endif
