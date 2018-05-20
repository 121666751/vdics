#ifndef _LINUX_TB_VOUCHERS_H
#define _LINUX_TB_VOUCHERS_H

struct tb_vouchers_s
{
	unsigned long sid;
	char fname[80];
	char bname[80];
	int rotate;
	char ip[24];
	short dpi;
	char date[10];
	char exchno[16];
	char pkgno[16];
	short session;
	short state;
	char vchno[31];
	char accno[32];
	char vchtype [4];
	char amount[16];
	short model;
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
