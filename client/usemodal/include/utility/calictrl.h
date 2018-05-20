#ifndef __CALICTRL_H__
#define __CALICTRL_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @param outCalX: buffer that receive cali-val X of the scanner
 * @param outCalY: buffer that receive cali-val Y of the scanner
 * @return 0-success.

 * read the cali-val from Windows Terminal
 */
int mac_getcalival(double* outCalX, double* outCalY);

/**
 * @param calX: cali-val X of the scanner	
 * @param calY: cali-val Y of the scanner	
 * @return 0-success.

 * write the cali-val from Windows Terminal
 */
int mac_setcalival(double calX, double calY);

/**
 * judge if in terminal mode
 */
bool is_terminal_mode();


/**
 * get mac address
 * @param strMac: the address to store the string of mac
 * @param ulMac: the address to store the unsigned long of the mac
 * @return 0-success
 */
int GetClientMac(char strMac[13], unsigned long ulMac[2]);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif //__CALICTRL_H__