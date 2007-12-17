# Mongoose - This is a simple dep line generator
#
# Call it passing CFLAGS and a source filename
# to generate a dep line

printf "# deps.sh was passed:\n"
printf "# "
echo $@

printf "# Then made this line:\n"
printf "\$(BUILDDIR)/"
cc -MM $@
printf "\t \$(DO_CC)\n\n"
