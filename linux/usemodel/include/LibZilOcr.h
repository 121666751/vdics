#ifndef LIBZILOCR_H
#define LIBZILOCR_H

#ifdef __cplusplus
extern "C" {
#endif

char * __stdcall zilocr_transact_file(const char* imgfilename_bmp);
char * _stdcall zilocr_transact_buffer(const unsigned char* img_buf, int size_x, int size_y,const char* fname);
void __stdcall zilocr_free(char *buf);
void __stdcall zilocr_set_milsys(void* milsysid);
int __stdcall zilocr_get_subimage(unsigned char* pSrcImage, int srcWidth, int srcHeight, int bands,
				 int offsetX, int offsetY, int subWidth, int subHeight, unsigned char* pDestImageBuf);


#ifdef __cplusplus
}
#endif

#endif

