# Mongoose - This is a simple dep line generator
#
# Call it passing CFLAGS and a source filename
# to generate a dep line

printf "\$(BUILD_DIR)/"
cc -MM $@
printf "\t \$(DO_CC)\n\n"
