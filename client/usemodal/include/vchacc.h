#ifndef __VCHACC_H__
#define __VCHACC_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ͨ��ƾ֤���뷴���ʺ�
 * @param vchno: ƾ֤��
 * @param vchtype: ƾ֤���ʹ���
 * @param outAccno: [out]�����߷�����ַ�����ַ�������洢�ͻ��ʺš�Ӧ
   �������40�ֽڵĿռ䡣
 * @return 0-�ɹ�
 */
int vch_numtoacc(const char* vchno, int vchtype, char* outAccno);

#ifdef __cplusplus
}
#endif

#endif //__VCHACC_H__