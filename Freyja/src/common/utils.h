
#ifndef strdup
  char *strdup(const char *src);
  /*------------------------------------------------------
   * Pre  : Vaild src string
   * Post : Returns duplicate string of the string 
   *        Returns NULL on error
   *
   *-- History ------------------------------------------
   *
   * 2000.09.09: 
   * Mongoose - Created from old mtk routine I wrote
   ------------------------------------------------------*/
#endif

  int extcheck(const char *filename, const char *ext);
  /*------------------------------------------------------
   * Pre  : Vaild filename and file extention 
   * Post : Returns 0 if the ext of filename matches
   *
   *-- History ------------------------------------------
   *
   * 2000.09.09: 
   * Mongoose - Created from old mtk routine I wrote
   ------------------------------------------------------*/
