/*
 ***************************************************************************
 * ×÷Õß   Nie
 * ÍøÖ·£º www.tchzt.com
 * ÓÊÏä£º niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#ifndef __ZBASE_SPECIALS_H__
#define __ZBASE_SPECIALS_H__

typedef struct tagSPECIALS
{
	int shape;
	int process;
	int x0;
	int y0;
	int x1;
	int y1;
}TIL_SPECIALS,*LPTIL_SPECIALS;
typedef list<TIL_SPECIALS> TIL_SPECIALSLIST, *LPTIL_TIL_SPECIALSLIST;

typedef struct tagIDRESNODE
{
	char	idx[20];
	int		iSysRes;
	int		iManRes;
	int		iSizeX;
	int		iSizeY;	
	int		iType;
	int		iDpi;
	int		iOffx;
	int		iOffy;
	int		iEndx;
	int		iEndy;
	int		iRstval;
	double	coff;
}TIL_IDRESNODE,*LPTIL_IDRESNODE;

typedef list<TIL_IDRESNODE> TIL_IDRESNODELIST, *LPTIL_TIL_IDRESNODELIST;

#endif // __ZBASE_SPECIALS_H__