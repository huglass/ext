//this file contain module function and global data definition
#include "sample_ext.h"

#include "sample_g.h"


// registered module resource type ID
int le_sample_descriptor;
int le_sample_descriptor_persist;



void php_sample_descriptor_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	FILE *fp = (FILE*)rsrc->ptr;
	fclose(fp);
}


void php_sample_descriptor_dtor_persistent(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	php_sample_descriptor_data *fdata =
		(php_sample_descriptor_data*)rsrc->ptr;
	fclose(fdata->fp);
	pefree(fdata->filename, 1);
	pefree(fdata, 1);
}




//////////////////////////////////////////////////////////
/*
module init function and shutdown function
*/

PHP_MINIT_FUNCTION(sample)
{
	le_sample_descriptor = 
		zend_register_list_destructors_ex(
		php_sample_descriptor_dtor, NULL, PHP_SAMPLE_DESCRIPTOR_RES_NAME, module_number);

	le_sample_descriptor_persist = 
		zend_register_list_destructors_ex(
		NULL, php_sample_descriptor_dtor_persistent, PHP_SAMPLE_DESCRIPTOR_RES_NAME, module_number);

	return SUCCESS;
}



