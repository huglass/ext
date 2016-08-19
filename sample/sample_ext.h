#ifndef PHP_SAMPLE_EXT_H
/* Prevent double inclusion */
#define PHP_SAMPLE_EXT_H

/* Define Extension Properties */
#define PHP_SAMPLE_EXTNAME    "sample"
#define PHP_SAMPLE_EXTVER    "1.0"



/* Import configure options
   when building outside of
   the PHP source tree */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Include PHP Standard Header */
#include "php.h"


/* Define the entry point symbol
 * Zend will use when loading this module
 */
extern zend_module_entry sample_module_entry;
#define phpext_sample_ptr &sample_module_entry

#endif /* PHP_SAMPLE_EXT_H */

