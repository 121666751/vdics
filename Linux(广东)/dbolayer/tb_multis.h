#ifndef _LINUX_TB_MULTIS_H
#define _LINUX_TB_MULTIS_H

struct tb_multis_s
{
	unsigned long sid;
	char fname[80];
	char bname[80];
	int rotate;
	char ip[24];
	short dpi;
	char date[30];
	char exchno[16];
	char pkgno[16];
	short session;
	short state;
	char vchno[12];
	char accno[32];
	char vchtype [4];
	short multiflag;
	char amount[16];
	short model;
	unsigned long mainid;
	unsigned long envid;
	short mainflag;
	short checkstate;
	short mergestate;
	unsigned long mergeid;
	char imageid[80];
	short sendstate;
	char timestamp[24];
	short submitstate;
	char scantime[24];
	short prestate;
	char clkno[12];
	short times;
	char area[9];
	char modeflag[2];
};

#endif
