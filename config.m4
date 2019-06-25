dnl $Id$
dnl config.m4 for extension dbr

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(dbr, for dbr support,
dnl Make sure that the comment is aligned:
dnl [  --with-dbr             Include dbr support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(dbr, whether to enable dbr support,
dnl Make sure that the comment is aligned:
[  --enable-dbr           Enable dbr support])

if test "$PHP_DBR" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-dbr -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/dbr.h"  # you most likely want to change this
  dnl if test -r $PHP_DBR/$SEARCH_FOR; then # path given as parameter
  dnl   DBR_DIR=$PHP_DBR
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for dbr files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       DBR_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$DBR_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the dbr distribution])
  dnl fi

  dnl # --with-dbr -> add include path
  dnl PHP_ADD_INCLUDE($DBR_DIR/include)

  dnl # --with-dbr -> check for lib and symbol presence
  LIBNAME=DynamsoftBarcodeReader
  LIBSYMBOL=DBR_CreateInstance

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
  PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, /usr/lib, DBR_SHARED_LIBADD)
  AC_DEFINE(HAVE_DBRLIB,1,[ ])
  ],[
  AC_MSG_ERROR([wrong dbr lib version or lib not found])
  ],[
  -L$DBR_DIR/$PHP_LIBDIR -lm
  ])
  
  PHP_SUBST(DBR_SHARED_LIBADD)
  PHP_NEW_EXTENSION(dbr, dbr.c, $ext_shared)
fi
