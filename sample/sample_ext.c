// this file contain module entry definiton
// must included first
#include "sample_ext.h"

// include global definition about the extension module
#include "sample_g.h"

// include extension implemetation function declaration
#include "sample_imp.h"



//////////////////////////////////////////////////////////
/*
arginfo:
argument hinting information for actual private function definition,
and these information is used as third parameter of PHP_FE in zend_function_entry,
*/

#ifdef ZEND_ENGINE_2
ZEND_BEGIN_ARG_INFO(php_sample_byref_arginfo, 0)
ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(php_sample_array_arginfo, 0)
//ZEND_ARG_INFO(1, arg)
ZEND_ARG_ARRAY_INFO(1, "arr", 0)
ZEND_END_ARG_INFO()
#else
static
unsigned char php_sample_byref_arginfo[] = {1, BYREF_FORCE};
#endif


//////////////////////////////////////////////////////////
/*
function entry point vector
PHP_FE associates function name in user space with internal c function name
*/
static zend_function_entry php_sample_functions[] = {
	PHP_FE(sample_hello_world,						NULL)
	PHP_NAMED_FE(sample_hi,							PHP_FN(sample_hello_world),						NULL)
	PHP_FALIAS(sample_hihi,							sample_hello_world,								NULL)
	PHP_FE(sample_long,								NULL)
	PHP_FE(sample_array_range,						NULL)
	PHP_FE(sample_byref_calltime,					NULL)
	PHP_FE(sample_byref_compiletime,				php_sample_byref_arginfo)
	PHP_FE(sample_hello_name,						NULL)
	PHP_FE(sample_greet_name,						NULL)
	PHP_FE(sample_optinal_greet_name,				NULL)
	PHP_FE(sample_var_dump,							NULL)
	PHP_FE(php_sample_print_var_hash,				php_sample_array_arginfo)
	PHP_FE(php_sample_print_var_hash_preserve,		php_sample_array_arginfo)
	PHP_FE(sample_return_array,						NULL)
	PHP_FE(sample_fopen,							NULL)
	PHP_FE(sample_fwrite,							NULL)
	PHP_FE(sample_fwrite_checkself,					NULL)
	PHP_FE(sample_fclose,							NULL)
	PHP_FE(sample_fopen_ex,							NULL)
	PHP_FE(sample_fclose_p,							NULL)
	{NULL,NULL,NULL}
};


//////////////////////////////////////////////////////////
/*
zend module definition
*/

zend_module_entry sample_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_SAMPLE_EXTNAME,
	php_sample_functions,
	PHP_MINIT(sample), /* MINIT */
	NULL, /* MSHUTDOWN */
	NULL, /* RINIT */
	NULL, /* RSHUTDOWN */
	NULL, /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
	PHP_SAMPLE_EXTVER,
#endif
	STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_SAMPLE
ZEND_GET_MODULE(sample)
#endif