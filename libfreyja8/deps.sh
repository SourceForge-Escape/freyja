# Mongoose - This is a simple dep line generator
#
# Call it passing CFLAGS and a source filename
# to generate a dep line

echo -n "\$(BUILDDIR)/"
cc -MM $@
printf "\t \$(DO_CC)\n\n"