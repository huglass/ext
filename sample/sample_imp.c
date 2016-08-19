//this file contains extension function implemetation
#include "sample_ext.h"

#include "sample_g.h"

//////////////////////////////////////////////////////////

//press warning: implicit declaration of function php_var_dump
#include "ext/standard/php_var.h"




//////////////////////////////////////////////////////////
/*
some functions called by php extension function
*/


int sample_strvec_handler(int argc, char **argv TSRMLS_DC)
{
    HashTable *ht;
    /* Allocate a block of memory
     * for the HashTable structure */
    ALLOC_HASHTABLE(ht);
    /* Initialize its internal state */
    if (zend_hash_init(ht, argc, NULL, ZVAL_PTR_DTOR, 0) == FAILURE) {
        FREE_HASHTABLE(ht);
        return FAILURE;
    }
    /* Populate each string into a zval* */
    while (argc > 0) {
        zval *value;
        MAKE_STD_ZVAL(value);
        ZVAL_STRING(value, argv[--argc], 1);
        argv++;
        if (zend_hash_next_index_insert(ht, (void**)&value, sizeof(zval*), NULL) == FAILURE) {
            /* Silently skip failed additions */
            zval_ptr_dtor(&value);
        }
    }
    /* Do some work */
    ///////////////////process_hashtable(ht);
    /* Destroy the hashtable
     * freeing all zval allocations as necessary */
    zend_hash_destroy(ht);

    /* Free the HashTable itself */
    FREE_HASHTABLE(ht);
    return SUCCESS;
}




//////////////////////////////////////////////////////////
/*
actual private function definition under php extension function name
*/

//php -r 'sample_hello_world();'
PHP_FUNCTION(sample_hello_world)
{
	MY_FI(sample_hello_world);
    php_printf("hello world\n");
}

//php -r 'var_dump(sample_long());'
PHP_FUNCTION(sample_long)
{
	//php_printf("#function:sample_long\n");
	MY_FI(sample_long);
	ZVAL_LONG(return_value, 42);
	RETVAL_LONG(42);
	return;
}


//php -r 'var_dump(sample_array_range());'
PHP_FUNCTION(sample_array_range)
{
	MY_FI(sample_array_range);
	if (return_value_used) {
		int i;
		array_init(return_value);
		for (i = 0; i < 1000; i++) {
			add_next_index_long(return_value, i);
		}
		return;
	} else {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "static return-only function called without processing output");
		RETURN_NULL();
	}
}

PHP_FUNCTION(sample_byref_calltime)
{
	MY_FI(sample_byref_calltime);
	zval *a;
	int addtl_len = sizeof(" (modified by ref!)") - 1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &a) == FAILURE) {
		RETURN_NULL();
	}

    if (!a->is_ref__gc) {
        /* parameter was not passed by reference,
         * leave without doing anything
         */
        return;
    }

    /* Make sure the variable is a string */
    convert_to_string(a);
    /* Enlarge a's buffer to hold the additional data */
    Z_STRVAL_P(a) = erealloc(Z_STRVAL_P(a),
        Z_STRLEN_P(a) + addtl_len + 1);
    memcpy(Z_STRVAL_P(a) + Z_STRLEN_P(a),
    " (modified by ref!)", addtl_len + 1);
    Z_STRLEN_P(a) += addtl_len;
}

//php -r '$a="default";sample_byref_compiletime($a);var_dump($a);'
PHP_FUNCTION(sample_byref_compiletime)
{
	MY_FI(sample_byref_compiletime);
	zval *a;
	int addtl_len = sizeof(" (modified by ref!)") - 1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &a) == FAILURE) {
		RETURN_NULL();
	}

    if (!a->is_ref__gc) {
        /* parameter was not passed by reference,
         * leave without doing anything
         */
        return;
    }

    /* Make sure the variable is a string */
    convert_to_string(a);
    /* Enlarge a's buffer to hold the additional data */
    Z_STRVAL_P(a) = erealloc(Z_STRVAL_P(a),
        Z_STRLEN_P(a) + addtl_len + 1);
    memcpy(Z_STRVAL_P(a) + Z_STRLEN_P(a),
    " (modified by ref!)", addtl_len + 1);
    Z_STRLEN_P(a) += addtl_len;
}

//php -r 'sample_hello_name("John");'
PHP_FUNCTION(sample_hello_name)
{
	MY_FI(sample_hello_name);
	char *name;
	int name_len;


	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
		&name, &name_len) == FAILURE) {
			RETURN_NULL();
	}
	php_printf("Hello ");
	PHPWRITE(name, name_len);
	php_printf("!\n");
}

//php -r 'sample_greet_name("John", "Mr.");'
PHP_FUNCTION(sample_greet_name)
{
	MY_FI(sample_greet_name);
	char *name;
	int name_len;
	char *greeting;
	int greeting_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss",
		&name, &name_len, &greeting, &greeting_len) == FAILURE) {
			RETURN_NULL();
	}
	php_printf("Hello ");
	PHPWRITE(greeting, greeting_len);
	php_printf(" ");
	PHPWRITE(name, name_len);
	php_printf("!\n");
}

//php -r 'sample_optinal_greet_name("John", "Mr.");'
//php -r 'sample_optinal_greet_name("John");'
PHP_FUNCTION(sample_optinal_greet_name)
{
	MY_FI(sample_optinal_greet_name);
	char *name;
	int name_len;
	char *greeting = "Mr./Mrs.";
	int greeting_len = sizeof("Mr./Mrs.") - 1;


	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s",
		&name, &name_len, &greeting, &greeting_len) == FAILURE) {
			RETURN_NULL();
	}
	php_printf("Hello ");
	PHPWRITE(greeting, greeting_len);
	php_printf(" ");
	PHPWRITE(name, name_len);
	php_printf("!\n");
}

//php -r 'sample_var_dump(1);'
PHP_FUNCTION(sample_var_dump)
{
	MY_FI(sample_var_dump);
	int i, argc = ZEND_NUM_ARGS();
	zval ***args;

	args = (zval ***)safe_emalloc(argc, sizeof(zval **), 0);
	if (ZEND_NUM_ARGS() == 0 ||
		zend_get_parameters_array_ex(argc, args) == FAILURE) {
			efree(args);
			WRONG_PARAM_COUNT;
	}
	for (i=0; i<argc; i++) {
		php_var_dump(args[i], 1 TSRMLS_CC);
	}
	efree(args);
}

//Emulating a foreach() loop without preserving the Internal Pointer
/*
php -r '$a=array('1'=>'xiao','2'=>'mi','3'=>'hao');next($a);echo current($a), PHP_EOL;php_sample_print_var_hash($a);if(current($a)===false){echo "inner pointer is changed",PHP_EOL;}'
*/
PHP_FUNCTION(php_sample_print_var_hash)
{
	zval *pzval = NULL;
	HashTable *arrht = NULL;
	/*
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "H", &arrht) == FAILURE
		|| arrht == NULL) {
		return;
	}
	*/
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &pzval) == FAILURE
		|| pzval == NULL
		|| Z_TYPE_P(pzval) != IS_ARRAY) {
			return;
	} else {
		if ((arrht = Z_ARRVAL_P(pzval)) == NULL) {
			return;
		}
	}

    for(zend_hash_internal_pointer_reset(arrht);
    zend_hash_has_more_elements(arrht) == SUCCESS;
    zend_hash_move_forward(arrht)) {
        char *key;
        uint keylen;
        ulong idx;
        int type;
        zval **ppzval, tmpcopy;

        type = zend_hash_get_current_key_ex(arrht, &key, &keylen, &idx, 0, NULL);
        if (zend_hash_get_current_data(arrht, (void**)&ppzval) == FAILURE) {
            /* Should never actually fail
             * since the key is known to exist. */
            continue;
        }
        /* Duplicate the zval so that
         * the orignal's contents are not destroyed */
        tmpcopy = **ppzval;
        zval_copy_ctor(&tmpcopy);
        /* Reset refcount & Convert */
        INIT_PZVAL(&tmpcopy);
        convert_to_string(&tmpcopy);
        /* Output */
        php_printf("The value of ");
        if (type == HASH_KEY_IS_STRING) {
            /* String Key / Associative */
            PHPWRITE(key, keylen);
        } else {
            /* Numeric Key */
            php_printf("%ld", idx);
        }
        php_printf(" is: ");
        PHPWRITE(Z_STRVAL(tmpcopy), Z_STRLEN(tmpcopy));
        php_printf("\n");
        /* Toss out old copy */
        zval_dtor(&tmpcopy);
    }
}

//Emulating a foreach() loop with preserving the Internal Pointer
/*
php -r '$a=array('1'=>'xiao','2'=>'mi','3'=>'hao');next($a);echo current($a), PHP_EOL;php_sample_print_var_hash_preserve($a);echo current($a), PHP_EOL;'
*/
PHP_FUNCTION(php_sample_print_var_hash_preserve)
{
	HashTable *arrht;
	//zval **entry;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "H", &arrht) == FAILURE
		|| arrht == NULL) {
		return;
	}

    HashPosition pos;
    for(zend_hash_internal_pointer_reset_ex(arrht, &pos);
    zend_hash_has_more_elements_ex(arrht, &pos) == SUCCESS;
    zend_hash_move_forward_ex(arrht, &pos)) {
        char *key;
        uint keylen;
        ulong idx;
        int type;

        zval **ppzval, tmpcopy;

        type = zend_hash_get_current_key_ex(arrht, &key, &keylen, &idx, 0, &pos);
        if (zend_hash_get_current_data_ex(arrht, (void**)&ppzval, &pos) == FAILURE) {
            /* Should never actually fail
             * since the key is known to exist. */
            continue;
        }
        /* Duplicate the zval so that
         * the original's contents are not destroyed */
        tmpcopy = **ppzval;
        zval_copy_ctor(&tmpcopy);
        /* Reset refcount & Convert */
        INIT_PZVAL(&tmpcopy);
        convert_to_string(&tmpcopy);
        /* Output */
        php_printf("The value of ");
        if (type == HASH_KEY_IS_STRING) {
            /* String Key / Associative */
            PHPWRITE(key, keylen);
        } else {
            /* Numeric Key */
            php_printf("%ld", idx);
        }
        php_printf(" is: ");
        PHPWRITE(Z_STRVAL(tmpcopy), Z_STRLEN(tmpcopy));
        php_printf("\n");
        /* Toss out old copy */
        zval_dtor(&tmpcopy);
    }
}

// return a array to user space
// php -r 'var_dump(sample_return_array());'
PHP_FUNCTION(sample_return_array)
{
	zval *subarray;

	array_init(return_value);
	/* Add some scalars */
	add_assoc_long(return_value, "life", 42);
	add_index_bool(return_value, 123, 1);
	add_next_index_double(return_value, 3.1415926535);
	/* Toss in a static string, dup'd by PHP */
	add_next_index_string(return_value, "Foo", 1);
	/* Now a manually dup'd string */
	add_next_index_string(return_value, estrdup("Bar"), 0);

	/* Create a subarray */
	MAKE_STD_ZVAL(subarray);
	array_init(subarray);
	/* Populate it with some numbers */
	add_next_index_long(subarray, 1);
	add_next_index_long(subarray, 20);
	add_next_index_long(subarray, 300);
	/* Place the subarray in the parent */
	add_index_zval(return_value, 444, subarray);

}

// open a non-persitent file resource
// php -r '$f=sample_FOPEN("/home/huglass/dev/phpstudy/ext/sample/test/123.txt","w+");var_dump($f);'
/*
php -r '
$f=sample_FOPEN("/home/huglass/dev/phpstudy/ext/sample/test/123.txt","r");
var_dump($f);'
*/
PHP_FUNCTION(sample_fopen)
{
	FILE *fp;
	char *filename, *mode;
	int filename_len, mode_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss",
		&filename, &filename_len,
		&mode, &mode_len) == FAILURE) {
			RETURN_NULL();
	}
	if (!filename_len || !mode_len) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING,
			"Invalid filename or mode length");
		RETURN_FALSE;
	}
	fp = fopen(filename, mode);
	if (!fp) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING,
			"Unable to open %s using mode %s",
			filename, mode);
		RETURN_FALSE;
	}
	ZEND_REGISTER_RESOURCE(return_value, fp, le_sample_descriptor);
}

// write a non-persistent file resource
// php -r '$f=sample_FOPEN("/home/huglass/dev/phpstudy/ext/sample/test/123.txt","w+");var_dump(sample_fwrite($f,"test"));'
PHP_FUNCTION(sample_fwrite)
{
    FILE *fp;
    zval *file_resource;
    char *data;
    int data_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs",
            &file_resource, &data, &data_len) == FAILURE ) {
        RETURN_NULL();
    }
    /* Use the zval* to verify the resource type and
     * retrieve its pointer from the lookup table */
    ZEND_FETCH_RESOURCE(fp, FILE*, &file_resource, -1,
        PHP_SAMPLE_DESCRIPTOR_RES_NAME, le_sample_descriptor);
    /* Write the data, and
     * return the number of bytes which were
     * successfully written to the file */
    RETURN_LONG(fwrite(data, 1, data_len, fp));
}

// difference between this and sample_fwrite is whether rsrc-type check is writen yourself
// php -r '$f=sample_FOPEN("/home/huglass/dev/phpstudy/ext/sample/test/123.txt","w+");var_dump(sample_fwrite_checkself($f,"good"));'
PHP_FUNCTION(sample_fwrite_checkself)
{
	FILE *fp;
	zval *file_resource;
	char *data;
	int data_len, rsrc_type;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs",
		&file_resource, &data, &data_len) == FAILURE ) {
			RETURN_NULL();
	}
	fp = (FILE*)zend_list_find(Z_RESVAL_P(file_resource), &rsrc_type);
	if (!fp || rsrc_type != le_sample_descriptor) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING,
			"Invalid resource provided");
		RETURN_FALSE;
	}
	RETURN_LONG(fwrite(data, 1, data_len, fp));
}

// write a non-persistent or persistent file resource
//PHP_FUNCTION(sample_fwrite_ex)

// Forcing Destruction
/*

*/
PHP_FUNCTION(sample_fclose)
{
    FILE *fp;
    zval *file_resource;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r",
                        &file_resource) == FAILURE ) {
        RETURN_NULL();
    }
    /* While it's not necessary to actually fetch the
     * FILE* resource, performing the fetch provides
     * an opportunity to verify that we are closing
     * the correct resource type. */
    ZEND_FETCH_RESOURCE(fp, FILE*, &file_resource, -1,
        PHP_SAMPLE_DESCRIPTOR_RES_NAME, le_sample_descriptor);
    /* Force the resource into self-destruct mode */
    zend_hash_index_del(&EG(regular_list),
                    Z_RESVAL_P(file_resource));
	INFO("del normal resource\n");
    RETURN_TRUE;
}

/*
php -r '
$a=sample_fclose_p("/home/huglass/dev/phpstudy/ext/sample/test/123.txt","w+");
'
*/
PHP_FUNCTION(sample_fclose_p)
{
	MY_FI(sample_fclose_p);
    FILE *fp;
    zval *file_resource;
	int name_len, hash_key_len, mode_len;
	char *f_name = NULL, *hash_key = NULL, *mode = NULL;
	zend_rsrc_list_entry *existing_file;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss",
          &f_name, &name_len, &mode, &mode_len) == FAILURE ) {
        RETURN_NULL();
    }
	/* Try to find an already opened file */
	hash_key_len = spprintf(&hash_key, 0, "sample_descriptor:%s:%s", f_name, mode);
	if (zend_hash_find(&EG(persistent_list), hash_key,hash_key_len + 1, (void **)&existing_file) == FAILURE) {
			efree(hash_key);
			RETURN_FALSE;
	}
	INFO("find existed persisitent resource\n");
    /* Force the resource into self-destruct mode */
	if (zend_hash_del(&EG(persistent_list), hash_key, hash_key_len + 1) == FAILURE) {
		efree(hash_key);
		RETURN_FALSE;
	}

	INFO("del existed persisitent resource\n");
	efree(hash_key);
    RETURN_TRUE;
}

// first try to get an persistent resource conditionally, if has, return
// second open a new resource, if it is persistent, insert into regular_list, assign to return value, and insert into persisitent_list
/* php -r '
	$a=sample_fopen_ex("/home/huglass/dev/phpstudy/ext/sample/test/123.txt","w+", true);
	$b=sample_fopen_ex("/home/huglass/dev/phpstudy/ext/sample/test/123.txt","w+", true);
	$c=sample_fclose_p("/home/huglass/dev/phpstudy/ext/sample/test/123.txt","w+");
'
php -r '
$a=sample_fopen_ex("/home/huglass/dev/phpstudy/ext/sample/test/123.txt","w+", true);
'
*/
PHP_FUNCTION(sample_fopen_ex)
{
	php_sample_descriptor_data *fdata;
	FILE *fp;
	char *filename, *mode, *hash_key;
	int filename_len, mode_len, hash_key_len;
	zend_bool persist = 0;
	zend_rsrc_list_entry *existing_file;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"ss|b",
		&filename, &filename_len, &mode, &mode_len,
		&persist) == FAILURE) {
			RETURN_NULL();
	}
	if (!filename_len || !mode_len) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING,
			"Invalid filename or mode length");
		RETURN_FALSE;
	}
	/* Try to find an already opened file */
	hash_key_len = spprintf(&hash_key, 0,
		"sample_descriptor:%s:%s", filename, mode);
	if (persist && zend_hash_find(&EG(persistent_list), hash_key,
		hash_key_len + 1, (void **)&existing_file) == SUCCESS) {
			INFO("existed persisitent resource\n");
			/* There's already a file open, return that! */
			ZEND_REGISTER_RESOURCE(return_value,
				existing_file->ptr, le_sample_descriptor_persist);
			efree(hash_key);
			return;
	}
	fp = fopen(filename, mode);
	if (!fp) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING,
			"Unable to open %s using mode %s",
			filename, mode);
		RETURN_FALSE;
	}
	if (!persist) {
		fdata = emalloc(sizeof(php_sample_descriptor_data));
		fdata->filename = estrndup(filename, filename_len);
		fdata->fp = fp;
		ZEND_REGISTER_RESOURCE(return_value, fdata,
			le_sample_descriptor);
		INFO("regular resource per request\n");
	} else {
		zend_rsrc_list_entry le;
		fdata = pemalloc(sizeof(php_sample_descriptor_data),1);
		fdata->filename = pemalloc(filename_len + 1, 1);
		memcpy(fdata->filename, filename, filename_len + 1);
		fdata->fp = fp;
		ZEND_REGISTER_RESOURCE(return_value, fdata,
			le_sample_descriptor_persist);
		/* Store a copy in the persistent_list */
		le.type = le_sample_descriptor_persist;
		le.ptr = fdata;
		/* hash_key has already been created by now */
		zend_hash_update(&EG(persistent_list),
			hash_key, hash_key_len + 1,
			(void*)&le, sizeof(zend_rsrc_list_entry), NULL);
		INFO("persisitent resource\n");
	}
	efree(hash_key);
}









