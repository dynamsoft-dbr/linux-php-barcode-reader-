/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_dbr.h"

#include "If_DBR.h"
#include "BarcodeFormat.h"
#include "BarcodeStructs.h"
#include "ErrorCode.h"
#include <stdbool.h>

/* If you declare any globals in php_dbr.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(dbr)
*/

/* True global resources - no need for thread safety here */
static int le_dbr;

/* {{{ dbr_functions[]
 *
 * Every user visible function must have an entry in dbr_functions[].
 */
const zend_function_entry dbr_functions[] = {
	PHP_FE(DecodeBarcodeFile,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in dbr_functions[] */
};
/* }}} */

/* {{{ dbr_module_entry
 */
zend_module_entry dbr_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"dbr",
	dbr_functions,
	PHP_MINIT(dbr),
	PHP_MSHUTDOWN(dbr),
	PHP_RINIT(dbr),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(dbr),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(dbr),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_DBR_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_DBR
ZEND_GET_MODULE(dbr)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("dbr.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_dbr_globals, dbr_globals)
    STD_PHP_INI_ENTRY("dbr.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_dbr_globals, dbr_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_dbr_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_dbr_init_globals(zend_dbr_globals *dbr_globals)
{
	dbr_globals->global_value = 0;
	dbr_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(dbr)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(dbr)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(dbr)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(dbr)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(dbr)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "dbr support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


// Barcode format
const char * GetFormatStr(__int64 format)
{
	if (format == CODE_39)
		return "CODE_39";
	if (format == CODE_128)
		return "CODE_128";
	if (format == CODE_93)
		return "CODE_93";
	if (format == CODABAR)
		return "CODABAR";
	if (format == ITF)
		return "ITF";
	if (format == UPC_A)
		return "UPC_A";
	if (format == UPC_E)
		return "UPC_E";
	if (format == EAN_13)
		return "EAN_13";
	if (format == EAN_8)
		return "EAN_8";
	if (format == INDUSTRIAL_25)
		return "INDUSTRIAL_25";
	if (format == QR_CODE)
		return "QR_CODE";
	if (format == PDF417)
		return "PDF417";
	if (format == DATAMATRIX)
		return "DATAMATRIX";

	return "UNKNOWN";
}

PHP_FUNCTION(DecodeBarcodeFile)
{
	array_init(return_value);

	// Get Barcode image path
	char* pFileName = NULL;
	bool isNativeOuput = false;
	bool isLogOn = false;
	int iLen = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sb|b", &pFileName, &iLen, &isNativeOuput, &isLogOn) == FAILURE) {
        RETURN_STRING("Invalid parameters", true);
    }

	if (isLogOn)
	{
		printf("params: %s, %d, %d\n", pFileName, iLen, isNativeOuput);
	}
	
	// Dynamsoft Barcode Reader: init
	__int64 llFormat = (OneD | QR_CODE | PDF417 | DATAMATRIX);
	int iMaxCount = 0x7FFFFFFF;
	int iIndex = 0;
	ReaderOptions ro = {0};
	pBarcodeResultArray pResults = NULL;
	int iRet = -1;
	char * pszTemp = NULL;

	// Initialize license
	iRet = DBR_InitLicense("84D34246FC1BC4BDD4078D71FCB5A3AA");
	printf("DBR_InitLicense ret: %d\n", iRet);
	ro.llBarcodeFormat = llFormat;
	ro.iMaxBarcodesNumPerPage = iMaxCount;

	// Decode barcode image
	int ret = DBR_DecodeFile(pFileName, &ro, &pResults);
	if (ret == DBR_OK)
	{
		int count = pResults->iBarcodeCount;
		pBarcodeResult* ppBarcodes = pResults->ppBarcodes;
		pBarcodeResult tmp = NULL;
		char result[2048] = {0};
		int i = 0;
		if (count == 0)
		{
			add_next_index_string(return_value, "No Barcode detected", true);
		}

		// loop all results
		for (; i < count; i++)
		{
			char barcodeResult[1024];

			// A barcode result.
			tmp = ppBarcodes[i];
			{
				// Working with PHP array: http://php.net/manual/en/internals2.variables.arrays.php
				zval *tmp_array;
				// Initialize zval
				MAKE_STD_ZVAL(tmp_array);
				array_init(tmp_array);
				// Add format & value to an array
				add_next_index_string(tmp_array, GetFormatStr(tmp->llFormat), true);
				add_next_index_string(tmp_array, tmp->pBarcodeData, true);
				// Add result to returned array
				add_next_index_zval(return_value, tmp_array);
			}
		}

		// Dynamsoft Barcode Reader: release memory
		DBR_FreeBarcodeResults(&pResults);

		if (isLogOn && isNativeOuput)
		{
			printf("Native result: %s\n", result);
		}

		if (isNativeOuput)
		{
			add_next_index_string(return_value, result, true);
		}
	}
	else
	{
		add_next_index_string(return_value, "No Barcode detected", true);
	}

}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
