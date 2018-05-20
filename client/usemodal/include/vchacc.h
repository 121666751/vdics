#ifndef __VCHACC_H__
#define __VCHACC_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 通过凭证号码反查帐号
 * @param vchno: 凭证号
 * @param vchtype: 凭证类型代码
 * @param outAccno: [out]调用者分配的字符串地址，用来存储客户帐号。应
   分配大于40字节的空间。
 * @return 0-成功
 */
int vch_numtoacc(const char* vchno, int vchtype, char* outAccno);

#ifdef __cplusplus
}
#endif

#endif //__VCHACC_H__