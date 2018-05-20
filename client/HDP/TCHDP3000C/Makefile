all:
	devenv TCHDP3000C.sln /rebuild release
	cd Release
	copy  "TCHDP3000C.dll" "../../install" /Y
	copy "TCHDP3000C.lib" "../../lib" /Y
	cd ..
	copy "TC_Hdp.h"           "../include" /Y
	coff2omf ./Release/TCHDP3000C.lib  ../lib/TCHDP3000C_b.lib
	rd /s /q "Release"
	
	