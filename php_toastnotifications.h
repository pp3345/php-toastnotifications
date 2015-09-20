#ifndef PHP_TOASTNOTIFICATIONS_H
#define PHP_TOASTNOTIFICATIONS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

extern "C" zend_module_entry toastnotifications_module_entry;
#define phpext_toastnotifications_ptr &toastnotifications_module_entry

#ifdef ZTS
#include "TSRM.h"
#endif

/*
ZEND_BEGIN_MODULE_GLOBALS(toastnotifications)
ZEND_END_MODULE_GLOBALS(toastnotifications)
*/

#define TNG(v) ZEND_MODULE_GLOBALS_ACCESSOR(toastnotifications, v)

#define TOAST_NOTIFICATION_TEMPLATE_MIN 1
#define TOAST_NOTIFICATION_TEMPLATE_TEXT_01 1
#define TOAST_NOTIFICATION_TEMPLATE_TEXT_02 2
#define TOAST_NOTIFICATION_TEMPLATE_TEXT_03 3
#define TOAST_NOTIFICATION_TEMPLATE_TEXT_04 4
#define TOAST_NOTIFICATION_TEMPLATE_IMAGE_AND_TEXT_01 5
#define TOAST_NOTIFICATION_TEMPLATE_IMAGE_AND_TEXT_02 6
#define TOAST_NOTIFICATION_TEMPLATE_IMAGE_AND_TEXT_03 7
#define TOAST_NOTIFICATION_TEMPLATE_IMAGE_AND_TEXT_04 8
#define TOAST_NOTIFICATION_TEMPLATE_MAX 8

#if defined(ZTS) && defined(COMPILE_DL_TOASTNOTIFICATIONS)
ZEND_TSRMLS_CACHE_EXTERN();
#endif

#endif