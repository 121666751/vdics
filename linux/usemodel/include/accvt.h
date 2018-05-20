#ifndef __ACCVT_H__
#define __ACCVT_H__

#ifdef __cpluscplus
extern "C" {
#endif

/**
 * convert account from e13b into real account
 */
typedef struct ACC_CONVERT_INFO
{
	char exchno[20];	/**< exchno session */
	char accpart[20];	/**< account session */
	char depno[20];		/**< depno of current clerk */
	char e13b[60];		/**< the orginal e13b code */
	char reserved[40];	/**< not used */
}ACC_CONVERT_INFO;


/**
 * information to help convert
 */
struct accextinfo_t
{
	char depno[20];
	char reserved[200];
};


/**
 * @param exchno  :		交换号
 * @param account :     局部帐号
 * @param fullAcc :		[out] 返回完整帐号
 * @return: 0 - success.
 *
 * convert account from e13b into real account
 */
int __stdcall acc_convert(const char* exchno, const char* account, char *fullAcc);


/**
 * @param info:		[in] account info to be convert.
 * @param outNetno: [out]netno after convert.
 * @param outAccFull: [out]full account after convert.
 * @return: 0 - success.
 *
 * convert account from e13b into real account
 */
int __stdcall acc_convert_ex(ACC_CONVERT_INFO* info, char* outNetno, char* outAccFull);


/**
 * @param info: information to help account no convert
 */
void __stdcall acc_set_info(accextinfo_t &info);


#ifdef __cpluscplus
}
#endif

#endif //__ACCVT_H__