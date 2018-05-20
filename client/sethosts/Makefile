all:
	bpr2mak setHosts.bpr -osetHosts.mak
	make -fsetHosts.mak
	rm *.obj
	rm *.mak
	cp setHosts.exe ../../install
	
