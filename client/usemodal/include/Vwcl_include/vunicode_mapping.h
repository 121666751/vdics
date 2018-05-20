/* VWCL - The Virtual Windows Class Library.
Copyright (C) 1996-2000 The VWCL Alliance

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.

Primary Author of this source code file:  Todd Osborne (todd@vwcl.org)
Other Author(s) of this source code file:
*/

#ifndef VUNICODE_MAPPING
#define VUNICODE_MAPPING

/*********************************************************************
This header defines VWCL macro's for C runtime functions that
can work with UNICODE or ASCII strings. This method is preferred over
including the Windows tchar.h header file, since those methods and
macro's are not available on UNIX systems, and because it defines
many non-standard (in the ANSI C sense) methods. Another reason for
this is that the C runtime, and vendor additions, lack consistency in
naming functions and parameters. Some simply end the function name in
w for the wide character version, while others put the w, or some
other marking, in the middle of the name. We always take the native
ASCII version of a method, prepend V, uppercase it, and define the
UNICODE version with a trailing W. For the ASCII version of the
functions, we do the same thing, but append the letter A. For macro's
that don't end with W or A, they will be defined to be either the
UNICODE or ASCII version depending on whether or not VWCL_UNICODE is
defined. Not all macro's defined in this section will work on all C
runtime libraries. We have not selected the "Least Common Denominator"
and have included some non-standard extensions to standard C. In
almost all cases, the only time this will be an issue is when doing
UNICODE work on operating systems or C runtime libraries that lack
the methods required to work with UNICODE.
*********************************************************************/

/*********************************************************************
There are 3 main sections to this file. The first levels the playing
field, so to speak, across operating system and C runtime libraries.
The second defines UNICODE only versions of the CRT. The third defines
only ASCII/ANSI versions. The fourth, creates "standard" macro's that
are either the UNICODE or ASCII version, depending on whether or not
VWCL_UNICODE is defined.

Take for instance the CRT functin fgets(). The UNICODE version will
be named VFGETSW, the ASCII version is VFGETSA. If UNICODE is being
used, VFGETS will be mapped to VFGETSW. If UNICODE is not being used,
VFGETS will become VFGETSA.

IMPORTANT: Never change anything in only one section. Whenver a macro
is added, removed, fixed, etc. it must be done in all 4 sections to
maintain our sanity.
*********************************************************************/

/*******************************************************************
SECTION 1
First, level the playing field among operating systems and CRT's.
*******************************************************************/
#ifdef VWCL_TARGET_UNIX
	/* UNIX used strcasecmp() instead of stricmp(). We do now.*/
	#define	_stricmp strcasecmp
#endif

/****************************************
SECTION 2
This section defines macro's for UNICODE.
*****************************************/

/* Formatted I/O.*/
#define VPRINTFW			wprintf
#define VFPRINTFW			fwprintf
#define VSPRINTFW			swprintf
#define VSNPRINTFW			_snwprintf
#define VVPRINTFW			vwprintf
#define VVFPRINTFW			vfwprintf
#define VVSPRINTFW			vswprintf
#define VVSNPRINTFW			_vsnwprintf
#define VSCANFW				wscanf
#define VFSCANFW			fwscanf
#define VSSCANFW			swscanf
/* Unformatted I/O.*/
#define VFGETCW				fgetwc
#define VFGETCHARW			_fgetwchar
#define VFGETSW				fgetws
#define VFPUTCW				fputwc
#define VFPUTCHARW			_fputwchar
#define VFPUTSW				fputws
#define VGETCW				getwc
#define VGETCHARW			getwchar
#define VGETSW				_getws
#define VPUTCW				putwc
#define VPUTCHARW			putwchar
#define VPUTSW				_putws
#define VUNGETCW			ungetwc
/* String conversion.*/
#define VSTRTODW			wcstod
#define VSTRTOLW			wcstol
#define VSTRTOULW			wcstoul
#define VATOFW(str)			atof(VStringConverter(str))
#define VATOIW(str)			atoi(VStringConverter(str))
#define VATOLW(str)			atol(VStringConverter(str))
/* String functions.*/
#define VSTRCATW			wcscat
#define VSTRCHRW			wcschr
#define VSTRCPYW			wcscpy
#define VSTRCSPNW			wcscspn
#define VSTRLENW			wcslen
#define VSTRNCATW			wcsncat
#define VSTRNCPYW			wcsncpy
#define VSTRPBRKW			wcspbrk
#define VSTRRCHRW			wcsrchr
#define VSTRSPNW			wcsspn
#define VSTRSTRW			wcsstr
#define VSTRTOKW			wcstok
#define VSTRDUPW			_wcsdup
#define VSTRNSETW			_wcsnset
#define VSTRREVW			_wcsrev
#define VSTRSETW			_wcsset
#define VSTRCMPW			wcscmp
#define VSTRICMPW			_wcsicmp
#define VSTRCMP_NOCASEW		VSTRICMPW
#define VSTRNCMPW			wcsncmp
#define VSTRCOLLW			_wcscoll
#define VSTRICOLLW			_wcsicoll
#define VSTRNCOLLW			_wcsncoll
#define VSTRNICOLLW			_wcsnicoll
/* Execute functions.*/
#define VEXECLW				_wexecl
#define VEXECLEW			_wexecle
#define VEXECLPW			_wexeclp
#define VEXECPEW			_wexeclpe
#define VEXECVW				_wexecv
#define VEXECVEW			_wexecve
#define VEXECVPW			_wexecvp
#define VEXECVPEW			_wexecvpe
#define VSPAWNLW			_wspawnl
#define VSPAWNLEW			_wspawnle
#define VSPAWNLPW			_wspawnlp
#define VSPAWNLPEW			_wspawnlpe
#define VSPAWNVW			_wspawnv
#define VSPAWNVEW			_wspawnve
#define VSPAWNVPW			_wspawnvp
#define VSPAWNVPEW			_wspawnvpe
#define VSYSTEMW			_wsystem
/* Time functions.*/
#define VASCTIMEW			_wasctime
#define VCTIMEW				_wctime
#define VSTRDATEW			_wstrdate
#define VSTRTIMEW			_wstrtime
#define VUTIMEW				_wutime
#define VSTRFTIMEW			_wcsftime
/* Directory functions */
#define VCHDIRW				_wchdir
#define VGETCWDW			_wgetcwd
#define VGETDCWDW			_wgetdcwd
#define VMKDIRW				_wmkdir
#define VRMDIRW				_wrmdir
/* Environment and Path functions.*/
#define VFULLPATHW			_wfullpath
#define VGETENVW			_wgetenv
#define VMAKEPATHW			_wmakepath
#define VPUTENVW			_wputenv
#define VSEARCHENVW			_wsearchenv
#define VSPLITPATHW			_wsplitpath
/* STDIO functions.*/
#define VFDOPENW			_wfdopen
#define VFSOPENW			_wfsopen
#define VFOPENW				_wfopen
#define VFREOPENW			_wfreopen
#define VPERRORW			_wperror
#define VPOPENW				_wpopen
#define VTEMPNAMW			_wtempnam
#define VTMPNAMW			_wtmpnam
/* I/O functions.*/
#define VACCESSW			_waccess
#define VCHMODW				_wchmod
#define VCREATW				_wcreat
#define VFINDFIRSTW			_wfindfirst
#define VFINDFIRSTI64W		_wfindfirsti64
#define VFINDNEXTW			_wfindnext
#define VFINDNEXTI64W		_wfindnexti64
#define VMKTEMPW			_wmktemp
#define VOPENW				_wopen
#define VREMOVEW			_wremove
#define VRENAMEW			_wrename
#define VSOPENW				_wsopen
#define VUNLINKW			_wunlink
#define VFINDDATA_TW		_wfinddata_t
#define VFINDDATAI64_TW		_wfinddatai64_t
/* stat() functions.*/
#define VSTATW				_wstat
#define VSTATI64W			_wstati64
#define VSTATSTRUCTUREW		struct _stat
/* Locale functions.*/
#define VSETLOCALEW			_wsetlocale
/* "Logical-character" mappings.*/
#define VSTRDECW			_wcsdec
#define VSTRINCW			_wcsinc
#define VSTRNCNTW			_wcsncnt
#define VSTRNEXTCW			_wcsnextc
#define VSTRNINCW			_wcsninc
#define VSTRSPNPW			_wcsspnp
/* ctype functions.*/
#define VISALNUMW			iswalnum
#define VISALPHAW			iswalpha
#define VISASCIIW			iswascii
#define VISCNTRLW			iswcntrl
#define VISDIGITW			iswdigit
#define VISGRAPHW			iswgraph
#define VISLOWERW			iswlower
#define VISPRINTW			iswprint
#define VISPUNCTW			iswpunct
#define VISSPACEW			iswspace
#define VISUPPERW			iswupper
#define VISXDIGITW			iswxdigit
#define VTOUPPERW			towupper
#define VTOLOWERW			towlower

/*******************************************
SECTION 3
This section defines macro's for ASCII/ANSI.
********************************************/

/* Formatted I/O.*/
#define VPRINTFA			printf
#define VFPRINTFA			fprintf
#define VSPRINTFA			sprintf
#define VSNPRINTFA			_snprintf
#define VVPRINTFA			vprintf
#define VVFPRINTFA			vfprintf
#define VVSPRINTFA			vsprintf
#define VVSNPRINTFA			_vsnprintf
#define VSCANFA				scanf
#define VFSCANFA			fscanf
#define VSSCANFA			sscanf
/* Unformatted I/O.*/
#define VFGETCA				fgetc
#define VFGETCHARA			_fgetchar
#define VFGETSA				fgets
#define VFPUTCA				fputc
#define VFPUTCHARA			fputchar
#define VFPUTSA				fputs
#define VGETCA				getc
#define VGETCHARA			getchar
#define VGETSA				gets
#define VPUTCA				putc
#define VPUTCHARA			putchar
#define VPUTSA				puts
#define VUNGETCA			ungetc
/* String conversion.*/
#define VSTRTODA			strtod
#define VSTRTOLA			strtol
#define VSTRTOULA			strtoul
#define VATOFA				atof
#define VATOIA				atoi
#define VATOLA				atol
/* String functions.*/
#define VSTRCATA			strcat
#define VSTRCHRA			strchr
#define VSTRCPYA			strcpy
#define VSTRCSPNA			strcspn
#define VSTRLENA			strlen
#define VSTRNCATA			strncat
#define VSTRNCPYA			strncpy
#define VSTRPBRKA			strpbrk
#define VSTRRCHRA			strrchr
#define VSTRSPNA			strspn
#define VSTRSTRA			strstr
#define VSTRTOKA			strtok
#define VSTRDUPA			_strdup
#define VSTRNSETA			_strnset
#define VSTRREVA			_strrev
#define VSTRSETA			_strset
#define VSTRCMPA			strcmp

#ifdef __BORLANDC__
#define VSTRICMPA			strcmpi
#else
#define VSTRICMPA			_stricmp
#endif

#define VSTRCMP_NOCASEA		VSTRICMPA
#define VSTRNCMPA			strncmp
#define VSTRCOLLA			_strcoll
#define VSTRICOLLA			_stricoll
#define VSTRNCOLLA			_strncoll
#define VSTRNICOLLA			strnicoll
/* Execute functions */
#define VEXECLA				execl
#define VEXECLEA			execle
#define VEXECLPA			execlp
#define VEXECPEA			execlpe
#define VEXECVA				execv
#define VEXECVEA			execve
#define VEXECVPA			execvp
#define VEXECVPEA			execvpe
#define VSPAWNLA			spawnl
#define VSPAWNLEA			spawnle
#define VSPAWNLPA			spawnlp
#define VSPAWNLPEA			spawnlpe
#define VSPAWNVA			spawnv
#define VSPAWNVEA			spawnve
#define VSPAWNVPA			spawnvp
#define VSPAWNVPEA			spawnvpe
#define VSYSTEMA			system
/* Time functions.*/
#define VASCTIMEA			asctime
#define VCTIMEA				ctime
#define VSTRDATEA			strdate
#define VSTRTIMEA			strtime
#define VUTIMEA				utime
#define VSTRFTIMEA			strftime
/* Directory functions */
#define VCHDIRA				chdir
#define VGETCWDA			getcwd
#define VGETDCWDA			getdcwd
#define VMKDIRA				mkdir
#define VRMDIRA				rmdir
/* Environment and Path functions.*/
#define VFULLPATHA			_fullpath
#define VGETENVA			getenv
#define VMAKEPATHA			_makepath
#define VPUTENVA			putenv
#define VSEARCHENVA			_searchenv
#define VSPLITPATHA			_splitpath
/* STDIO functions.*/
#define VFDOPENA			fdopen
#define VFSOPENA			_fsopen
#define VFOPENA				fopen
#define VFREOPENA			freopen
#define VPERRORA			perror
#define VPOPENA				_popen
#define VTEMPNAMA			tempnam
#define VTMPNAMA			tmpnam
/* I/O functions.*/
#define VACCESSA			access
#define VCHMODA				chmod
#define VCREATA				creat
#define VFINDFIRSTA			_findfirst
#define VFINDFIRSTI64A		_findfirsti64
#define VFINDNEXTA			_findnext
#define VFINDNEXTI64A		_findnexti64
#define VMKTEMPA			mktemp
#define VOPENA				open
#define VREMOVEA			remove
#define VRENAMEA			rename
#define VSOPENA				_sopen
#define VUNLINKA			unlink
#define VFINDDATA_TA		_finddata_t
#define VFINDDATAI64_TA		_finddatai64_t
/* stat() functions.*/
#define VSTATA				stat
#define VSTATI64A			_stati64
#define VSTATSTRUCTUREA		struct stat
/* Locale functions.*/
#define VSETLOCALEA			setlocale
/* "Logical-character" mappings.*/
#define VSTRDECA			_strdec
#define VSTRINCA			_strinc
#define VSTRNCNTA			_strncnt
#define VSTRNEXTCA			_strnextc
#define VSTRNINCA			_strninc
#define VSTRSPNPA			_strspnp
/* ctype functions.*/
#define VISALNUMA			isalnum
#define VISALPHAA			isalpha
#define VISASCIIA			isascii
#define VISCNTRLA			iscntrl
#define VISDIGITA			isdigit
#define VISGRAPHA			isgraph
#define VISLOWERA			islower
#define VISPRINTA			isprint
#define VISPUNCTA			ispunct
#define VISSPACEA			isspace
#define VISUPPERA			isupper
#define VISXDIGITA			isxdigit
#define VTOUPPERA			toupper
#define VTOLOWERA			tolower

/************************************************
SECTION 4
This section map's macro's for ASCII and UNICODE.
************************************************/
#ifdef VWCL_UNICODE
	/* Formatted I/O.*/
	#define VPRINTF				VPRINTFW
	#define VFPRINTF			VFPRINTFW
	#define VSPRINTF			VSPRINTFW
	#define VSNPRINTF			VSNPRINTFW
	#define VVPRINTF			VVPRINTFW
	#define VVFPRINTF			VVFPRINTFW
	#define VVSPRINTF			VVSPRINTFW
	#define VVSNPRINTF			VVSNPRINTFW
	#define VSCANF				VSCANFW
	#define VFSCANF				VFSCANFW
	#define VSSCANF				VSSCANFW
	/* Unformatted I/O.*/
	#define VFGETC				VFGETCW
	#define VFGETCHAR			VFGETCHARW
	#define VFGETS				VFGETSW
	#define VFPUTC				VFPUTCW
	#define VFPUTCHAR			VFPUTCHARW
	#define VFPUTS				VFPUTSW
	#define VGETC				VGETCW
	#define VGETCHAR			VGETCHARW
	#define VGETS				VGETSW
	#define VPUTC				VPUTCW
	#define VPUTCHAR			VPUTCHARW
	#define VPUTS				VPUTSW
	#define VUNGETC				VUNGETCW
	/* String conversion.*/
	#define VSTRTOD				VSTRTODW
	#define VSTRTOL				VSTRTOLW
	#define VSTRTOUL			VSTRTOULW
	#define VATOF				VATOFW
	#define VATOI				VATOIW
	#define VATOL				VATOLW
	/* String functions.*/
	#define VSTRCAT				VSTRCATW
	#define VSTRCHR				VSTRCHRW
	#define VSTRCPY				VSTRCPYW
	#define VSTRCSPN			VSTRCSPNW
	#define VSTRLEN				VSTRLENW
	#define VSTRNCAT			VSTRNCATW
	#define VSTRNCPY			VSTRNCPYW
	#define VSTRPBRK			VSTRPBRKW
	#define VSTRRCHR			VSTRRCHRW
	#define VSTRSPN				VSTRSPNW
	#define VSTRSTR				VSTRSTRW
	#define VSTRTOK				VSTRTOKW
	#define VSTRDUP				VSTRDUPW
	#define VSTRNSET			VSTRNSETW
	#define VSTRREV				VSTRREVW
	#define VSTRSET				VSTRSETW
	#define VSTRCMP				VSTRCMPW
	#define VSTRICMP			VSTRICMPW
	#define VSTRCMP_NOCASE		VSTRCMP_NOCASEW
	#define VSTRNCMP			VSTRNCMPW
	#define VSTRCOLL			VSTRCOLLW
	#define VSTRICOLL			VSTRICOLLW
	#define VSTRNCOLL			VSTRNCOLLW
	#define VSTRNICOLL			VSTRNICOLLW
	/* Execute functions */
	#define VEXECL				VEXECLW
	#define VEXECLE				VEXECLEW
	#define VEXECLP				VEXECLPW
	#define VEXECPE				VEXECPEW
	#define VEXECV				VEXECVW
	#define VEXECVE				VEXECVEW
	#define VEXECVP				VEXECVPW
	#define VEXECVPE			VEXECVPEW
	#define VSPAWNL				VSPAWNLW
	#define VSPAWNLE			VSPAWNLEW
	#define VSPAWNLP			VSPAWNLPW
	#define VSPAWNLPE			VSPAWNLPEW
	#define VSPAWNV				VSPAWNVW
	#define VSPAWNVE			VSPAWNVEW
	#define VSPAWNVP			VSPAWNVPW
	#define VSPAWNVPE			VSPAWNVPEW
	#define VSYSTEM				VSYSTEMW
	/* Time functions.*/
	#define VASCTIME			VASCTIMEW
	#define VCTIME				VCTIMEW
	#define VSTRDATE			VSTRDATEW
	#define VSTRTIME			VSTRTIMEW
	#define VUTIME				VUTIMEW
	#define VSTRFTIME			VSTRFTIMEW
	/* Directory functions */
	#define VCHDIR				VCHDIRW
	#define VGETCWD				VGETCWDW
	#define VGETDCWD			VGETDCWDW
	#define VMKDIR				VMKDIRW
	#define VRMDIR				VRMDIRW
	/* Environment and Path functions.*/
	#define VFULLPATH			VFULLPATHW
	#define VGETENV				VGETENVW
	#define VMAKEPATH			VMAKEPATHW
	#define VPUTENV				VPUTENVW
	#define VSEARCHENV			VSEARCHENVW
	#define VSPLITPATH			VSPLITPATHW
	/* STDIO functions.*/
	#define VFDOPEN				VFDOPENW
	#define VFSOPEN				VFSOPENW
	#define VFOPEN				VFOPENW
	#define VFREOPEN			VFREOPENW
	#define VPERROR				VPERRORW
	#define VPOPEN				VPOPENW
	#define VTEMPNAM			VTEMPNAMW
	#define VTMPNAM				VTMPNAMW
	/* I/O functions.*/
	#define VACCESS				VACCESSW
	#define VCHMOD				VCHMODW
	#define VCREAT				VCREATW
	#define VFINDFIRST			VFINDFIRSTW
	#define VFINDFIRSTI64		VFINDFIRSTI64W
	#define VFINDNEXT			VFINDNEXTW
	#define VFINDNEXTI64		VFINDNEXTI64W
	#define VMKTEMP				VMKTEMPW
	#define VOPEN				VOPENW
	#define VREMOVE				VREMOVEW
	#define VRENAME				VRENAMEW
	#define VSOPEN				VSOPENW
	#define VUNLINK				VUNLINKW
	#define VFINDDATA_T			VFINDDATA_TW
	#define VFINDDATAI64_T		VFINDDATAI64_TW
	/* stat() functions.*/
	#define VSTAT				VSTATW
	#define VSTATI64			VSTATI64W
	#define VSTATSTRUCTURE		VSTATSTRUCTUREW
	/* Locale functions.*/
	#define VSETLOCALE			VSETLOCALEW
	/* "Logical-character" mappings.*/
	#define VSTRDEC				VSTRDECW
	#define VSTRINC				VSTRINCW
	#define VSTRNCNT			VSTRNCNTW
	#define VSTRNEXTC			VSTRNEXTCW
	#define VSTRNINC			VSTRNINCW
	#define VSTRSPNP			VSTRSPNPW
	/* ctype functions.*/
	#define VISALNUM			VISALNUMW
	#define VISALPHA			VISALPHAW
	#define VISASCII			VISASCIIW
	#define VISCNTRL			VISCNTRLW
	#define VISDIGIT			VISDIGITW
	#define VISGRAPH			VISGRAPHW
	#define VISLOWER			VISLOWERW
	#define VISPRINT			VISPRINTW
	#define VISPUNCT			VISPUNCTW
	#define VISSPACE			VISSPACEW
	#define VISUPPER			VISUPPERW
	#define VISXDIGIT			VISXDIGITW
	#define VTOUPPER			VTOUPPERW
	#define VTOLOWER			VTOLOWERW
#else
	/* Formatted I/O.*/
	#define VPRINTF				VPRINTFA
	#define VFPRINTF			VFPRINTFA
	#define VSPRINTF			VSPRINTFA
	#define VSNPRINTF			VSNPRINTFA
	#define VVPRINTF			VVPRINTFA
	#define VVFPRINTF			VVFPRINTFA
	#define VVSPRINTF			VVSPRINTFA
	#define VVSNPRINTF			VVSNPRINTFA
	#define VSCANF				VSCANFA
	#define VFSCANF				VFSCANFA
	#define VSSCANF				VSSCANFA
	/* Unformatted I/O.*/
	#define VFGETC				VFGETCA
	#define VFGETCHAR			VFGETCHARA
	#define VFGETS				VFGETSA
	#define VFPUTC				VFPUTCA
	#define VFPUTCHAR			VFPUTCHARA
	#define VFPUTS				VFPUTSA
	#define VGETC				VGETCA
	#define VGETCHAR			VGETCHARA
	#define VGETS				VGETSA
	#define VPUTC				VPUTCA
	#define VPUTCHAR			VPUTCHARA
	#define VPUTS				VPUTSA
	#define VUNGETC				VUNGETCA
	/* String conversion.*/
	#define VSTRTOD				VSTRTODA
	#define VSTRTOL				VSTRTOLA
	#define VSTRTOUL			VSTRTOULA
	#define VATOF				VATOFA
	#define VATOI				VATOIA
	#define VATOL				VATOLA
	/* String functions.*/
	#define VSTRCAT				VSTRCATA
	#define VSTRCHR				VSTRCHRA
	#define VSTRCPY				VSTRCPYA
	#define VSTRCSPN			VSTRCSPNA
	#define VSTRLEN				VSTRLENA
	#define VSTRNCAT			VSTRNCATA
	#define VSTRNCPY			VSTRNCPYA
	#define VSTRPBRK			VSTRPBRKA
	#define VSTRRCHR			VSTRRCHRA
	#define VSTRSPN				VSTRSPNA
	#define VSTRSTR				VSTRSTRA
	#define VSTRTOK				VSTRTOKA
	#define VSTRDUP				VSTRDUPA
	#define VSTRNSET			VSTRNSETA
	#define VSTRREV				VSTRREVA
	#define VSTRSET				VSTRSETA
	#define VSTRCMP				VSTRCMPA
	#define VSTRICMP			VSTRICMPA
	#define VSTRCMP_NOCASE		VSTRCMP_NOCASEA
	#define VSTRNCMP			VSTRNCMPA
	#define VSTRCOLL			VSTRCOLLA
	#define VSTRICOLL			VSTRICOLLA
	#define VSTRNCOLL			VSTRNCOLLA
	#define VSTRNICOLL			VSTRNICOLLA
	/* Execute functions */
	#define VEXECL				VEXECLA
	#define VEXECLE				VEXECLEA
	#define VEXECLP				VEXECLPA
	#define VEXECPE				VEXECPEA
	#define VEXECV				VEXECVA
	#define VEXECVE				VEXECVEA
	#define VEXECVP				VEXECVPA
	#define VEXECVPE			VEXECVPEA
	#define VSPAWNL				VSPAWNLA
	#define VSPAWNLE			VSPAWNLEA
	#define VSPAWNLP			VSPAWNLPA
	#define VSPAWNLPE			VSPAWNLPEA
	#define VSPAWNV				VSPAWNVA
	#define VSPAWNVE			VSPAWNVEA
	#define VSPAWNVP			VSPAWNVPA
	#define VSPAWNVPE			VSPAWNVPEA
	#define VSYSTEM				VSYSTEMA
	/* Time functions.*/
	#define VASCTIME			VASCTIMEA
	#define VCTIME				VCTIMEA
	#define VSTRDATE			VSTRDATEA
	#define VSTRTIME			VSTRTIMEA
	#define VUTIME				VUTIMEA
	#define VSTRFTIME			VSTRFTIMEA
	/* Directory functions */
	#define VCHDIR				VCHDIRA
	#define VGETCWD				VGETCWDA
	#define VGETDCWD			VGETDCWDA
	#define VMKDIR				VMKDIRA
	#define VRMDIR				VRMDIRA
	/* Environment and Path functions.*/
	#define VFULLPATH			VFULLPATHA
	#define VGETENV				VGETENVA
	#define VMAKEPATH			VMAKEPATHA
	#define VPUTENV				VPUTENVA
	#define VSEARCHENV			VSEARCHENVA
	#define VSPLITPATH			VSPLITPATHA
	/* STDIO functions.*/
	#define VFDOPEN				VFDOPENA
	#define VFSOPEN				VFSOPENA
	#define VFOPEN				VFOPENA
	#define VFREOPEN			VFREOPENA
	#define VPERROR				VPERRORA
	#define VPOPEN				VPOPENA
	#define VTEMPNAM			VTEMPNAMA
	#define VTMPNAM				VTMPNAMA
	/* I/O functions.*/
	#define VACCESS				VACCESSA
	#define VCHMOD				VCHMODA
	#define VCREAT				VCREATA
	#define VFINDFIRST			VFINDFIRSTA
	#define VFINDFIRSTI64		VFINDFIRSTI64A
	#define VFINDNEXT			VFINDNEXTA
	#define VFINDNEXTI64		VFINDNEXTI64A
	#define VMKTEMP				VMKTEMPA
	#define VOPEN				VOPENA
	#define VREMOVE				VREMOVEA
	#define VRENAME				VRENAMEA
	#define VSOPEN				VSOPENA
	#define VUNLINK				VUNLINKA
	#define VFINDDATA_T			VFINDDATA_TA
	#define VFINDDATAI64_T		VFINDDATAI64_TA
	/* stat() functions.*/
	#define VSTAT				VSTATA
	#define VSTATI64			VSTATI64A
	#define VSTATSTRUCTURE		VSTATSTRUCTUREA
	/* Locale functions.*/
	#define VSETLOCALE			VSETLOCALEA
	/* "Logical-character" mappings.*/
	#define VSTRDEC				VSTRDECA
	#define VSTRINC				VSTRINCA
	#define VSTRNCNT			VSTRNCNTA
	#define VSTRNEXTC			VSTRNEXTCA
	#define VSTRNINC			VSTRNINCA
	#define VSTRSPNP			VSTRSPNPA
	/* ctype functions.*/
	#define VISALNUM			VISALNUMA
	#define VISALPHA			VISALPHAA
	#define VISASCII			VISASCIIA
	#define VISCNTRL			VISCNTRLA
	#define VISDIGIT			VISDIGITA
	#define VISGRAPH			VISGRAPHA
	#define VISLOWER			VISLOWERA
	#define VISPRINT			VISPRINTA
	#define VISPUNCT			VISPUNCTA
	#define VISSPACE			VISSPACEA
	#define VISUPPER			VISUPPERA
	#define VISXDIGIT			VISXDIGITA
	#define VTOUPPER			VTOUPPERA
	#define VTOLOWER			VTOLOWERA
#endif

#endif /* VUNICODE_MAPPING*/
