################################################################
# Mongoose <mongoose@icculus.org>
################################################################
# + Wrapper Makefile
################################################################

IMGS=./modules/image/*
MDLS=./modules/model/*
BACK=../../../

debug:
	for i in $(IMGS); do cd $$i && make debug && cd $(BACK) || cd $(BACK); done 
	for i in $(MDLS); do cd $$i && make debug && cd $(BACK) || cd $(BACK); done 

release:
	for i in $(IMGS); do cd $$i && make release && cd $(BACK) || cd $(BACK); done 
	for i in $(MDLS); do cd $$i && make release && cd $(BACK) || cd $(BACK); done 

win32:
	for i in $(IMGS); do cd $$i && make win32 && cd $(BACK) || cd $(BACK); done 
	for i in $(MDLS); do cd $$i && make win32 && cd $(BACK) || cd $(BACK); done 

osx:
	for i in $(IMGS); do cd $$i && make osx && cd $(BACK) || cd $(BACK); done 
	for i in $(MDLS); do cd $$i && make osx && cd $(BACK) || cd $(BACK); done 

clean:
	for i in $(IMGS); do cd $$i && make clean && cd $(BACK) || cd $(BACK); done 
	for i in $(MDLS); do cd $$i && make clean && cd $(BACK) || cd $(BACK); done 

