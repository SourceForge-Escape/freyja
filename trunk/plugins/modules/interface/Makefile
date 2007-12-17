################################################################
# Mongoose <mongoose@icculus.org>
################################################################
# + Wrapper Makefile
################################################################

BACK=../
RULE=

debug:
	$(MAKE) foreach RULE="debug"

release:
	$(MAKE) foreach RULE="release"

win32:
	$(MAKE) foreach RULE="win32"

clean:
	$(MAKE) foreach RULE="clean"

user-install: 
	$(MAKE) foreach RULE="user-install" 

foreach:
	for i in */; do cd $$i && $(MAKE) $(RULE) && cd $(BACK) || cd $(BACK); done


#################################################################