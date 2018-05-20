#ifndef __SCANCTL_H__
#define __SCANCTL_H__

struct scan_info_t {
	int st_ver;			// 0x0010
	long milsys;
    int  region_mode;    // 0-auto, 1-use below set
	int	 left;			// pixcel
	int  top;
	int  right;
	int  bottom;
        int  dpi;
        int  flag;	//0-pc,1-linux terminate ,2 wince.
	char reserved[72];
};

struct id_img_t; // defined in stmproc.h


extern "C" {

/*
 *	
 */
HANDLE scan_open(scan_info_t& info);

/*
 *	
 */
int scan_start(HANDLE h);

int scan_start_ex(HANDLE h,double left,double right,double top,double bottom,int resolution);


/*
 *	
 */
int scan_getimage(HANDLE h, id_img_t& image);
/*
 *	
 */

int scan_getzoomedimage(HANDLE h, id_img_t& image);

/*
 *	
 */
int scan_close(HANDLE h);

}

#endif //__SCANCTL_H__