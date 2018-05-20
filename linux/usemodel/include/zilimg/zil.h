#ifndef __ZIL_H
#define __ZIL_H

typedef  long 	ZIL_ID;

/************************************************************************/
/* general default parameters (may be bit encoded)                      */
/************************************************************************/
#undef	Z_NULL

#define Z_NULL                                        0x00000000L
#define Z_FALSE                                       0L
#define Z_NO                                          0L
#define Z_OFF                                         0L
#define Z_IN_PROGRESS                                 0L
#define Z_FINISHED                                    1L
#define Z_TRUE                                        1L
#define Z_YES                                         1L
#define Z_ON                                          1L
#define Z_WAIT                                        1L
#define Z_CREATE                                      Z_YES
#define Z_FREE                                        Z_NO
#define Z_DEFAULT                                     0x10000000L
#define Z_QUIET                                       0x08000000L
#define Z_VALID                                       0x00000001L
#define Z_INVALID                                     -1L
#define Z_CLEAR                                       0x00000001L
#define Z_NO_CLEAR                                    0x00000002L
#define Z_LUT_OFFSET                                  0x80000000L
#define Z_ENABLE                                      -9997L
#define Z_DISABLE                                     -9999L

#define Z_EXTENDED                                    0x80000000L            // remove
#define Z_EXTENDED_ATTRIBUTE                          Z_EXTENDED             // remove


/************************************************************************/
/* buffer ID offset for defaults                                        */
/************************************************************************/
#define Z_UNSIGNED                                    0x00000000L
#define Z_SIGNED                                      0x80000000L
#define Z_FLOAT                                      (0x40000000L | Z_SIGNED)
#define Z_DOUBLE                                     (0x20000000L | Z_SIGNED)
#define Z_SIZE_BIT_MASK                               0x000000FFL
#define Z_TYPE_MASK                                   0xFFFFFF00L

/************************************************************************/
/* MMX related                                                          */
/************************************************************************/
#define MMX_EXTRA_BYTES                               32

/************************************************************************/
/*	ZIL_ID type parameter												*/
/************************************************************************/
#define Z_APP_ID										0L
#define Z_SYS_ID										1L
#define Z_DIS_ID										2L
#define Z_IMG_ID										3L






/*******************************************************************/
/* ZbufImport(), ZbufExport(), ZbufJPEGExport()    				   */
/*******************************************************************/
#define Z_RESTORE										0L
#define Z_LOAD											1L

#define Z_ZIL											0L
#define Z_RAW 											1L
#define Z_TIFF											2L
#define Z_GIF											3L
#define Z_BMP											4L
#define Z_JPEG											5L


/*******************************************************************/
/* Color Bands Define							   				   */
/*******************************************************************/
#define Z_RED_BAND				0
#define Z_GREEN__BAND			1
#define Z_BLUE_BAND				2
#define Z_ALL_BANDS				1000
				

#endif