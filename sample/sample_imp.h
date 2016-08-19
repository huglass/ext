#ifndef PHP_SAMPLE_IMP_H
/* Prevent double inclusion */
#define PHP_SAMPLE_IMP_H


//////////////////////////////////////////////////////////
/*
actual private function declaration under php extension function name
*/

//php -r 'sample_hello_world();'
PHP_FUNCTION(sample_hello_world);

//php -r 'var_dump(sample_long());'
PHP_FUNCTION(sample_long);


//php -r 'var_dump(sample_array_range());'
PHP_FUNCTION(sample_array_range);

PHP_FUNCTION(sample_byref_calltime);

//php -r '$a="default";sample_byref_compiletime($a);var_dump($a);'
PHP_FUNCTION(sample_byref_compiletime);

//php -r 'sample_hello_name("John");'
PHP_FUNCTION(sample_hello_name);

//php -r 'sample_greet_name("John", "Mr.");'
PHP_FUNCTION(sample_greet_name);

//php -r 'sample_optinal_greet_name("John", "Mr.");'
//php -r 'sample_optinal_greet_name("John");'
PHP_FUNCTION(sample_optinal_greet_name);

//php -r 'sample_var_dump(1);'
PHP_FUNCTION(sample_var_dump);

//Emulating a foreach() loop without preserving the Internal Pointer
/*
php -r '$a=array('1'=>'xiao','2'=>'mi','3'=>'hao');next($a);echo current($a), PHP_EOL;php_sample_print_var_hash($a);if(current($a)===false){echo "inner pointer is changed",PHP_EOL;}'
*/
PHP_FUNCTION(php_sample_print_var_hash);

//Emulating a foreach() loop with preserving the Internal Pointer
/*
php -r '$a=array('1'=>'xiao','2'=>'mi','3'=>'hao');next($a);echo current($a), PHP_EOL;php_sample_print_var_hash_preserve($a);echo current($a), PHP_EOL;'
*/
PHP_FUNCTION(php_sample_print_var_hash_preserve);

// return a array to user space
// php -r 'var_dump(sample_return_array());'
PHP_FUNCTION(sample_return_array);

// open a non-persitent file resource
// php -r '$f=sample_FOPEN("/home/huglass/dev/phpstudy/ext/sample/test/123.txt","w+");var_dump($f);'
PHP_FUNCTION(sample_fopen);

// write a non-persistent file resource
// php -r '$f=sample_FOPEN("/home/huglass/dev/phpstudy/ext/sample/test/123.txt","w+");var_dump(sample_fwrite($f,"test"));'
PHP_FUNCTION(sample_fwrite);

// difference between this and sample_fwrite is whether rsrc-type check is writen yourself
// php -r '$f=sample_FOPEN("/home/huglass/dev/phpstudy/ext/sample/test/123.txt","w+");var_dump(sample_fwrite_checkself($f,"good"));'
PHP_FUNCTION(sample_fwrite_checkself);

// write a non-persistent or persistent file resource
//PHP_FUNCTION(sample_fwrite_ex)

// Forcing Destruction
PHP_FUNCTION(sample_fclose);

//get an existed resource or open a new resource
PHP_FUNCTION(sample_fopen_ex);

PHP_FUNCTION(sample_fclose_p);


#endif