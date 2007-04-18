################################################################
# Mongoose <mongoose@icculus.org>
################################################################
# + Wrapper Makefile
################################################################

IMGS=./modules/image/*
MDLS=./modules/model/*
UIPS=./modules/interface/*
BACK=../../../

debug:
	for i in $(IMGS); do cd $$i && make debug && cd $(BACK) || cd $(BACK); done 
	for i in $(MDLS); do cd $$i && make debug && cd $(BACK) || cd $(BACK); done 
	for i in $(UIPS); do cd $$i && make debug && cd $(BACK) || cd $(BACK); done 


release:
	for i in $(IMGS); do cd $$i && make release && cd $(BACK) || cd $(BACK); done 
	for i in $(MDLS); do cd $$i && make release && cd $(BACK) || cd $(BACK); done 
	for i in $(UIPS); do cd $$i && make release && cd $(BACK) || cd $(BACK); done 


win32:
	for i in $(IMGS); do cd $$i && make win32 && cd $(BACK) || cd $(BACK); done 
	for i in $(MDLS); do cd $$i && make win32 && cd $(BACK) || cd $(BACK); done 
	for i in $(UIPS); do cd $$i && make win32 && cd $(BACK) || cd $(BACK); done 

osx:
	for i in $(IMGS); do cd $$i && make osx && cd $(BACK) || cd $(BACK); done 
	for i in $(MDLS); do cd $$i && make osx && cd $(BACK) || cd $(BACK); done 
	for i in $(UIPS); do cd $$i && make osx && cd $(BACK) || cd $(BACK); done 


clean:
	for i in $(IMGS); do cd $$i && make clean && cd $(BACK) || cd $(BACK); done 
	for i in $(MDLS); do cd $$i && make clean && cd $(BACK) || cd $(BACK); done 
	for i in $(UIPS); do cd $$i && make clean && cd $(BACK) || cd $(BACK); done 


#################################################################
# Debian packages
#################################################################

install:
	@-mkdir -p $(DESTDIR)/usr/share/freyja/plugins
	@-cp ../bin/plugins/ui/debug/*.so $(DESTDIR)/usr/share/freyja/plugins
	@-cp ../bin/plugins/ui/debug/*.mlisp $(DESTDIR)/usr/share/freyja/plugins
	@-mkdir -p $(DESTDIR)/usr/share/freyja/plugins/model/
	@-cp ../bin/plugins/model/debug/*.so $(DESTDIR)/usr/share/freyja/plugins/model/
	@-mkdir -p $(DESTDIR)/usr/share/freyja/plugins/image/
	@-cp ../bin/plugins/image/debug/*.so $(DESTDIR)/usr/share/freyja/plugins/image/


install-old:
	for i in $(IMGS); do cd $$i && make install && cd $(BACK) || cd $(BACK); done 
	for i in $(MDLS); do cd $$i && make install && cd $(BACK) || cd $(BACK); done 
	for i in $(UIPS); do cd $$i && make install && cd $(BACK) || cd $(BACK); done 


#################################################################
# Debian packages
#################################################################

deb:
	dpkg-buildpackage -rfakeroot
	$(MAKE) deb-info

deb-info:
	@-printf "================================================================================\n"
	dpkg --info ../freyja-plugins_*.deb
	@-printf " -------------------------------------------------------------------------------\n"
	dpkg --contents ../freyja-plugins_*.deb
	@-printf "================================================================================\n"


#################################################################