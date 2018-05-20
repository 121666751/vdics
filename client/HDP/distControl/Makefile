all:
	devenv distControllor.sln /rebuild release 
	cd Release
	copy "distControllor.dll" "../../install" /Y
	copy "distControllor.lib" "../../lib/" /Y
	cd ..
	copy "distControllor.h"   "../include/"  /Y
	coff2omf ./release/distControllor.lib  ../lib/distControllor_b.lib
	del /s /q /f "Release\*.*"
	rd /s /q "Release"
	
	