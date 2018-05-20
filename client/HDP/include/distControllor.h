/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
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
 * 数据处理类初始化函数，返回数据处理类指针。
 * @Handle ：[in] 控制窗口句柄
 * @返回   ：数据处理类指针
 */
HANDLE    __stdcall InitDistCtrl( HWND Handle );

/*
 * 数据处理类释放函数，用于释放分配的内存
 * @ hDistCtrl：[in] 数据处理类指针，调用InitDistCtrl的返回值	
 */
void      __stdcall ReleaseDistCtrl( HANDLE Handle );
/*
 * 处理清分暂停，函数用于实现数据处理类和界面模块的同步。
 * @ hDistCtrl：[in] 数据处理类指针，调用InitDistCtrl的返回值
 * @返回    ：0-成功 其他-失败
 */
int       __stdcall PauseDist(HANDLE hDistCtrl);

/*
 * 处理清分停止，函数用于实现数据处理类和界面模块的同步。
 * @ hDistCtrl ：[in] 数据处理类指针，调用InitDistCtrl的返回值
 * @返回    ：0-成功 其他-失败	
 */
int       __stdcall StopDist(HANDLE hDistCtrl);
/*
 * 用于存储清分详细数据及状态
 * @hDistCtrl：[in] 数据处理类指针，调用InitDistCtrl的返回值
 * @返回    ：0-成功 其他-失败
 */
int	   	  __stdcall StoreDistData(HANDLE hDistCtrl);

/*
 * 返回当前处理票据的口袋号
 * @hDistCtrl：[in] 数据处理类指针，调用InitDistCtrl的返回值
 * @返回    ：0-成功 其他-失败	
 */
int       __stdcall GetPocketNo(HANDLE hDistCtrl);

/*
 * 返回当前处理票据的数据
 * @hDistCtrl：[in] 数据处理类指针，调用InitDistCtrl的返回值
 * @返回    ：当前票据数据结构体指针	
 */
DISTDATA* __stdcall GetOneData(HANDLE hDistCtrl);
/*
 * 将当前从清分控制模块中获得的票据信息存入到数据处理类数据链表中
 * @hDistCtrl：[in] 数据处理类指针，调用InitDistCtrl的返回值
 * @pData   ：[in] 票据信息
 * @返回    ：0-成功 其他-失败
 */
bool      __stdcall SetOneData( HANDLE hDistCtrl, DISTDATA *pData);
/*
 * 计算清分借、贷数据总额，总笔数
 * @hDistCtrl：[in] 数据处理类指针，调用InitDistCtrl的返回值
 * @dramount：[out] 借方总金额
 * @drcount  ：[out] 借方总笔数
 * @cramount：[out]  贷方总金额
 * @crcount  ：[out] 贷方总笔数
 * @返回    ：0-成功 其他-失败
 */
int       __stdcall CalTotalAmount(HANDLE hDistCtrl, double* dramount, int* drcount, 
							 double* cramount, int* crcount);
/*
 * 从链表中删除指定序号的数据
 * @hDistCtrl  : [in] 数据处理类指针，调用InitDistCtrl的返回值
 * @ightIndex  : [in] 序号
 * @返回    ：0-成功 其他-失败
 */
int       __stdcall DelSpecialItems(HANDLE hDistCtrl, int rightIndex);
/*
 * 返回所有的链表数据
 * @hDistCtrl：[in] 数据处理类指针，调用InitDistCtrl的返回值
 * @pData   ：[out] 返回链表数据
 * @count   ：[out] 返回链表数据总数
 * @返回    ：0-成功 其他-失败
 */
int       __stdcall GetAlltems(HANDLE hDistCtrl, DISTDATA* pData, int* count);

/*
 * 将当前从清分控制模块中获得的票据信息存入到数据处理类数据链表中
 * @hDistCtrl：[in] 数据处理类指针，调用InitDistCtrl的返回值
 * @pData   ：[in] 票据信息
 * @返回    ：0-成功 其他-失败
 */
int       __stdcall StartDstEx(HANDLE hDistCtrl,DISTCTRLDATA* pDcd);

#ifdef __cplusplus
}
#endif

#endif //__DISTCONTROLLOR_H__