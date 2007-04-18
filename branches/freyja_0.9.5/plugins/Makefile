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
	@-rm -f ../bin/plugins/model/debug
	@-mkdir -p ../bin/plugins/model/debug/
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
# Install rules
#################################################################

USER_DIR=$(HOME)/.freyja/plugins
SHARE_DIR=$(DESTDIR)/usr/share/freyja/plugins

install:
	@-mkdir -p $(SHARE_DIR)/
	@-cp ../bin/plugins/ui/debug/*.so $(SHARE_DIR)/
	@-cp ../bin/plugins/ui/debug/*.mlisp $(SHARE_DIR)/
	@-mkdir -p $(SHARE_DIR)/model/
	@-cp ../bin/plugins/model/debug/*.so $(SHARE_DIR)/model/
	@-mkdir -p $(SHARE_DIR)/image/
	@-cp ../bin/plugins/image/debug/*.so $(SHARE_DIR)/image/


user-install:
	@-mkdir -p $(USER_DIR)/
	@-cp ../bin/plugins/ui/debug/*.so $(USER_DIR)/
	@-cp ../bin/plugins/ui/debug/*.mlisp $(USER_DIR)/
	@-mkdir -p $(USER_DIR)/model/
	@-cp ../bin/plugins/model/debug/*.so $(USER_DIR)/model/
	@-mkdir -p $(USER_DIR)/image/
	@-cp ../bin/plugins/image/debug/*.so $(USER_DIR)/image/



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