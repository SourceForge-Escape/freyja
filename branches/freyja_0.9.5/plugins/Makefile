################################################################
# Mongoose <mongoose@icculus.org>
################################################################
# + Wrapper Makefile
################################################################

VERSION=0.9.5
IMGS=./modules/image/*
MDLS=./modules/model/*
UIPS=./modules/interface/*
BACK=../../../

debug:
	@-rm -f ../bin/plugins/model/debug
	@-mkdir -p ../bin/plugins/model/debug/
	$(MAKE) foreach RULE="debug" 


release:
	$(MAKE) foreach RULE="release" 


win32:
	$(MAKE) foreach RULE="win32" 
	for i in ../bin/plugins/model/*/win32; do cp $$i/*.dll ../bin/plugins/model/win32/; done


osx:
	$(MAKE) foreach RULE="osx" 


clean:
	$(MAKE) foreach RULE="clean" 


#user-install: 
#	$(MAKE) foreach RULE="user-install" 


foreach:
	for i in $(IMGS); do cd $$i && $(MAKE) $(RULE) && cd $(BACK) || cd $(BACK); done
	for i in $(MDLS); do cd $$i && $(MAKE) $(RULE) && cd $(BACK) || cd $(BACK); done
	cd modules/interface/ && $(MAKE) $(RULE) && cd $(BACK) || cd $(BACK)


#################################################################
# Install rules
#################################################################

USER_DIR=$(HOME)/.freyja/plugins
SHARE_DIR=$(DESTDIR)/usr/share/freyja/plugins
SELECT=debug

install:
	@-mkdir -p $(SHARE_DIR)/
	@-printf $(VERSION) > $(SHARE_DIR)/version
	@-cp ../bin/plugins/ui/$(SELECT)/*.so $(SHARE_DIR)/
	@-cp ../bin/plugins/ui/$(SELECT)/*.mlisp $(SHARE_DIR)/
	@-mkdir -p $(SHARE_DIR)/model/
	@-cp ../bin/plugins/model/$(SELECT)/*.so $(SHARE_DIR)/model/
	@-mkdir -p $(SHARE_DIR)/image/
	@-cp ../bin/plugins/image/$(SELECT)/*.so $(SHARE_DIR)/image/


user-install:
	@-mkdir -p $(USER_DIR)/
	@-cp ../bin/plugins/ui/$(SELECT)/*.so $(USER_DIR)/
	@-cp ../bin/plugins/ui/$(SELECT)/*.mlisp $(USER_DIR)/
	@-mkdir -p $(USER_DIR)/model/
	@-cp ../bin/plugins/model/$(SELECT)/*.so $(USER_DIR)/model/
	@-mkdir -p $(USER_DIR)/image/
	@-cp ../bin/plugins/image/$(SELECT)/*.so $(USER_DIR)/image/



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