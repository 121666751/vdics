/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#ifndef __TC_HDP_H__
#define __TC_HDP_H__

#ifdef __cplusplus
extern "C" {	
#endif
	/************************************************************************
	* 函数用于初始化控制清分机所用的动态库，同时传进两个处理模块，一个用于处理清分机获得的数据，一个用于错误处理，返回清分机控制类指针。
	* @hwnd ： [in] 控制窗口句柄
	* @proc ： [in] 数据处理模块，用于分析获得的micr,ocr等等
	* @perr ： [in] 处理错误信息
	* @返回 ： 清分机控制类指针                                                                     
	/************************************************************************/
	HANDLE 
		__stdcall InitMDL(HWND hwnd,void (*post_proc)(int num),void (*post_err)(int condition));		//Initialize TCHdpMDL and return its instance handle
	/*
	*	函数用于释放清分机控制实例，释放已加载的动态库等
	*  @handle ：清分机控制类指针，调用InitMDL 初始化函数的返回值
	*/
	void
		__stdcall FreeMDL(HANDLE handle);							//Release the TCHdpMDL instance
	/*
	* 清分机初始化函数，用于打开com口,初始化ocr识别引擎
	* @handle ：[in]清分机控制类指针，调用InitMDL 初始化函数的返回值
	* @返回   ：0-成功 其他-失败
	*/
	int
		__stdcall HDPOpen(HANDLE handle);							//Connect with HDP sorter
	/*
	* 清分机关闭化函数，用于关闭com口,关闭ocr识别引擎
	* @handle ：[in]清分机控制类指针，调用InitMDL 初始化函数的返回值
	* @返回   ：0-成功 其他-失败
	*/
	int
		__stdcall HDPClose(HANDLE handle);							//Disconnect with HDP sorter
	/*
	* 清分机运行函数，用于启动清分机进行清票
	* @handle   ：[in]清分机控制类指针，调用InitMDL 初始化函数的返回值
	* @mode     ：[in]清分模式  2-256灰度模式  3-24真彩色模式
	* @initflag ：[in]是否需要对内部参数进行初始化工作 true-需要 false-不需要
	* @返回     ：0-成功 其他-失败
	*/
	int
		__stdcall HDPStart(HANDLE handle,int mode, int speed, bool initflag); 				//Start Sort with HDP sorter

	int
		__stdcall HDPStop(HANDLE handle);							//Stop the HDP sorter
	/*
	* 函数用于获取需要存储的图像名。
	* @handle ：[in]清分机控制类指针，调用InitMDL 初始化函数的返回值
	* @fname  ：[in]需要保存的图像文件名
	* @返回   ：0-成功 其他-失败
	*/
	int
		__stdcall HDPGetImg(HANDLE handle, char *fname);			//Get voucher image from the HDP sorter
	/*
	* 函数用于硬件获取票据磁码信息
	* @handle ：[in]清分机控制类指针，调用InitMDL 初始化函数的返回值
	* @num    ：[in] 票据清分序号
	* @micr   ：[out] 票据磁码信息
	* @返回   ：0-成功 其他-失败
	*/
	int
		__stdcall HDPGetMicr(HANDLE handle, int num, char *micr);	//Get MICR code from the HDP sorter
	/*
	* 函数用于软件获取票据磁码信息
	* @handle ：[in]清分机控制类指针，调用InitMDL 初始化函数的返回值
	* @num    ：[in] 票据清分序号
	* @ocr    ：[out] 票据磁码信息
	* @返回   ：0-成功 其他-失败
	*/


	int	__stdcall HDPGetOcr(HANDLE handle, int num, char *ocr);

	/*
	* 函数用于软件获取票据磁码信息
	* @milSys ：[in]mil指针
	* @fname  ：[in] 图像名
	* @ocr    ：[out] 票据磁码信息
	* @返回   ：0-成功 其他-失败
	*/
int
 NewHDPGetOcr(char *fname,char*ocr);
/*
 * 函数用于设置口袋，将票据清分到指定口袋
 * @handle ：[in]清分机控制类指针，调用InitMDL 初始化函数的返回值
 * @pocket ：[in]口袋号
 * @返回   ：0-成功 其他-失败
 */



int
__stdcall HDPSetPocket(HANDLE handle, int pocket);			//Set the sort pocket of the HDP sorter
/*
 * 函数清除轨道中的票据
 * @handle ：[in]清分机控制类指针，调用InitMDL 初始化函数的返回值
 * @返回   ：0-成功 其他-失败
 */
int
__stdcall HDPPurge(HANDLE handle);							//Clear the current voucher in the HDP sorter
/*
 * 函数用于设置清分的基色
 * @handle ：[in]清分机控制类指针，调用InitMDL 初始化函数的返回值
 * @返回   ：0-成功 其他-失败
 */
int
__stdcall HDPBaseSet(HANDLE handle);						//Set the base color of the HDP sorter
/*
 * 设置清分机清分参数，如图像范围，扫描方式等
 * @handle ：[in]清分机控制类指针，调用InitMDL 初始化函数的返回值
 * @mode   ：[in] 色彩模式 2-256级灰度 3-彩色
 * @返回   ：0-成功 其他-失败	
 */
int
__stdcall HDPAdSet(HANDLE handle, int mode, int speed);		//Set the speed and wide of the HDP sorter
/*
 * 实现背书打印功能
 * @handle ：[in]清分机控制类指针，调用InitMDL 初始化函数的返回值
 * @str    ：[in] 需要打印的内容
 * @on     ：[in] 起始位置
 */
int
__stdcall HDPEndorse(HANDLE handle, char *str, int on);
/*
 * @消息处理函数，函数体由外部传入
 * @errcode :错误码
 */
void
__stdcall HDPErrMsg(int errcode);	//Return the error explaination of the HDP error code

int 
__stdcall HDPdecBankPtr(HANDLE handle);
/*
 * 函数用于将采集到的图像数据存到硬盘上。
 * @handle ：[in]清分机控制类指针，调用InitMDL 初始化函数的返回值
 * @index  ：[in]票据清分序号
 * @fname  ：[in]需要保存的正面图像文件名
 * @rname  ：[in]需要保存的反面图像文件名
 * @返回   ：0-成功 其他-失败
 */
int 
__stdcall HDPSaveImage(HANDLE handle,int index,char *fname,char *rname);

#ifdef __cplusplus
}
#endif

#endif //__TC_HDP_H__