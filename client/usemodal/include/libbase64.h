/**
 * @file
 *
 * 数据编码、解码、循环校验函数库
 */

#ifndef __LIBBASE64_H__ 
#define __LIBBASE64_H__

#ifdef __cplusplus
	extern "C"{
#endif


/**
 * 说明：将源数据按BASE64编码传出
 * 注意：需要在调用此函数之前开辟出存放目标数据的内存,内存大小为源数据长度的2倍,内存使用后由调用者自行释放.
 * 
 * @param src: 源数据
 * @param srclen: 源数据长度
 * @param dest:编码后的目标数据
 * @param msize:目标数据的长度
 * @return 0-成功, other-失败.
 */
int encode_base64(void *src,int srclen,char *dest,int *msize);


/**
 * 说明：将按BASE64编码的源数据解码
 * 注意：需要在调用此函数之前开辟出存放目标数据的内存,内存大小为源数据长度的1倍,内存使用后由调用者自行释放.
 * 
 * @param src: 编码的源数据串
 * @param srclen:编码的源数据串长度
 * @param dest:解码后的目标数据
 * @param msize:解码后的目标数据的长度
 * @return 0-成功, other-失败.
 */
int decode_base64(char *src,int srclen,void *dest,int *msize);


/**
 * 说明：将源数据按yencode编码传出
 * 注意：需要在调用此函数之前开辟出存放目标数据的内存,内存大小为源数据长度的2倍,内存使用后由调用者自行释放.
 * 
 * @param src: 源数据
 * @param srclen: 源数据长度
 * @param dest:编码后的目标数据
 * @param msize:目标数据的长度
 * @return 0-成功, other-失败.
 */
int yencode(void *src,int srclen,char **dest,int *msize);


/**
 * 说明：将按yencode编码的源数据解码
 * 注意：需要在调用此函数之前开辟出存放目标数据的内存,内存大小为源数据长度的1倍,内存使用后由调用者自行释放.
 * 
 * @param src: 编码的源数据串
 * @param srclen:编码的源数据串长度
 * @param dest:解码后的目标数据
 * @param msize:解码后的目标数据的长度
 * @return 0-成功, other-失败.
 */
int ydecode(char *src,int srclen,void **dest,int *msize);


/**
 * 说明：计算源数据的循环校验值

 * @param src: 源数据
 * @param srclen: 源数据长度
 * @param dst:循环校验值
 * @return 0-成功, other-失败.
 */
int CRC32_Digest(void *src,int srclen,unsigned int *dst);


/**
 * 说明：将源数据按一种编码转换为另一种编码
 * 注意：需要在调用此函数之前开辟出存放目标数据的内存,内存大小视编码不同而有所不同,内存使用后由调用者自行释放.
 * 
 * @param inbuf: 源数据
 * @param inlen: 源数据长度
 * @param outbuf:目标数据
 * @param outlen:目标数据的长度
 * @param fromcode:源数据编码
 * @param tocode:需要转换的数据编码
 * @return 0-成功, other-失败.
 */
int iconvMem(char *inbuf,int *inlen,char *outbuf,int *outlen,char *fromcode,char *tocode);


/**
 * 说明：将源数据按yencode编码,
 		 并以=ybegin filename=xxx size=xxx 开始, 并以=yend filename=xxx size=xxx CRC32=xxx结束,得到目标数据
		 其中Myencode函数直接对原始数据进行yencode编码,Myencode_ex函数则先将原始数据进行zlib压缩,然后在对压缩
		 数据编码.

 * 注意：需要在调用此函数之前开辟出存放目标数据的内存,内存大小为源数据长度的2倍+100,内存使用后由调用者自行释放.
 * 
 * @param src: 源数据
 * @param srclen: 源数据长度
 * @param filename:数据标识关键字
 * @param dest:编码后的目标数据(含前缀、后缀)
 * @param msize:目标数据的长度
 * @return 0-成功, other-失败.
 */
int Myencode(void *src,int srclen,char *filename,char *dst,int *dstlen);
int Myencode_ex(void *src,int srclen,char *filename,char *dst,int *dstlen);

/**
 * 说明: 得到解码数据长度
 * @param src: 待解码源数据
 * @return >0: 得到解码数据长度, -1: 源数据错误
 */
int MyGetDecodeSize(char* src);

/**
 * 说明：将按yencode编码,并以=ybegin filename=xxx size=xxx 开始,以=yend filename=xxx size=xxx CRC32=xxx结束的源数据,
 		 解码得到目标数据
 * 注意：需要在调用此函数之前开辟出存放目标数据的内存,内存大小须通过MyGetDecodeSize得到,
		 内存使用后由调用者自行释放.
 * 
 * @param src: 源数据(含前缀、后缀)
 * @param filename:数据标识关键字
 * @param dst:解码后的目标数据
 * @param dstlen:目标数据的长度
 * @return 0-成功, other-失败.
 */
int Mydecode(char *src,char *filename,void *dst,int *dstlen);

#ifdef __cplusplus
}

#endif

#endif
