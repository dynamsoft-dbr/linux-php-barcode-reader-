/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
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

#include "DynamsoftBarcodeReader.h"

/* If you declare any globals in php_dbr.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(dbr)
*/

/* True global resources - no need for thread safety here */
static int le_dbr;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("dbr.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_dbr_globals, dbr_globals)
    STD_PHP_INI_ENTRY("dbr.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_dbr_globals, dbr_globals)
PHP_INI_END()
*/
/* }}} */

static void *hBarcode = NULL;

#define CHECK_DBR() 										\
if (!hBarcode) 												\
{															\
	hBarcode = DBR_CreateInstance();						\
	const char* versionInfo = DBR_GetVersion();				\
	printf("Dynamsoft Barcode Reader %s\n", versionInfo);	\
}															


PHP_FUNCTION(DBRInitLicense)
{
	CHECK_DBR();

	char *pszLicense;
	size_t iLen;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &pszLicense, &iLen) == FAILURE)
	{
		RETURN_STRING("Invalid parameters");
	}
	DBR_InitLicense(hBarcode, pszLicense);
}

PHP_FUNCTION(DecodeBarcodeFile)
{
	CHECK_DBR();

	array_init(return_value);

	// Get Barcode image path
	char *pFileName;
	long barcodeType = 0;
	size_t iLen;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &pFileName, &iLen, &barcodeType) == FAILURE)
	{
		RETURN_STRING("Invalid parameters");
	}

	if (hBarcode)
	{
		int iMaxCount = 0x7FFFFFFF;
		TextResultArray *pResults = NULL;

		// Update DBR params
		PublicRuntimeSettings pSettings = {0};
		DBR_GetRuntimeSettings(hBarcode, &pSettings);
		pSettings.barcodeFormatIds = barcodeType;
		char szErrorMsgBuffer[256];
		DBR_UpdateRuntimeSettings(hBarcode, &pSettings, szErrorMsgBuffer, 256);

		// Barcode detection
		int ret = DBR_DecodeFile(hBarcode, pFileName, "");
		DBR_GetAllTextResults(hBarcode, &pResults);
		if (pResults)
		{
			int count = pResults->resultsCount;
			int i = 0;
			for (; i < count; i++)
			{
				zval tmp_array;
				array_init(&tmp_array);
				add_next_index_string(&tmp_array, pResults->results[i]->barcodeFormatString);
				add_next_index_string(&tmp_array, pResults->results[i]->barcodeText);
				add_next_index_stringl(&tmp_array, pResults->results[i]->barcodeBytes, pResults->results[i]->barcodeBytesLength);
				add_next_index_zval(return_value, &tmp_array);
			}
			DBR_FreeTextResults(&pResults);
		}
	}
}

PHP_FUNCTION(DBRCreate)
{
	CHECK_DBR();
}

PHP_FUNCTION(DBRDestroy)
{
	if (hBarcode)
	{
		DBR_DestroyInstance(hBarcode);
		hBarcode = NULL;
	}
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/

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
#if defined(COMPILE_DL_DBR) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
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

/* {{{ dbr_functions[]
 *
 * Every user visible function must have an entry in dbr_functions[].
 */
const zend_function_entry dbr_functions[] = {
	PHP_FE(DBRInitLicense, NULL)
	PHP_FE(DecodeBarcodeFile, NULL)
	PHP_FE(DBRCreate, NULL)
	PHP_FE(DBRDestroy, NULL)
	PHP_FE_END /* Must be the last line in dbr_functions[] */
};
/* }}} */

/* {{{ dbr_module_entry
 */
zend_module_entry dbr_module_entry = {
	STANDARD_MODULE_HEADER,
	"dbr",
	dbr_functions,
	PHP_MINIT(dbr),
	PHP_MSHUTDOWN(dbr),
	PHP_RINIT(dbr),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(dbr), /* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(dbr),
	PHP_DBR_VERSION,
	STANDARD_MODULE_PROPERTIES};
/* }}} */

#ifdef COMPILE_DL_DBR
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(dbr)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
