#include "php_toastnotifications.h"

#include <Zend\zend_exceptions.h>

extern "C" {
#include <ext\spl\spl_exceptions.h>
}

#include <Windows.h>
#include <roapi.h>
#include <wrl\client.h>
#include <windows.ui.notifications.h>

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace ABI::Windows::UI::Notifications;
using namespace ABI::Windows::Data::Xml::Dom;

/*
ZEND_DECLARE_MODULE_GLOBALS(toastnotifications)
*/

ZEND_METHOD(ToastNotification, __construct);
ZEND_METHOD(ToastNotification, display);

ZEND_BEGIN_ARG_INFO_EX(ToastNotification___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, "template", IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ToastNotification_display, 0, 0, 0)
ZEND_END_ARG_INFO()

const static zend_function_entry ToastNotificationMethods[] = {
	ZEND_ME(ToastNotification, __construct, ToastNotification___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(ToastNotification, display, ToastNotification_display, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

static zend_class_entry *ToastNotificationCE;

PHP_MINIT_FUNCTION(toastnotifications) {
	zend_class_entry ToastNotification;

	CoInitialize(NULL);

	INIT_CLASS_ENTRY(ToastNotification, "ToastNotification", ToastNotificationMethods);
	ToastNotificationCE = zend_register_internal_class(&ToastNotification);
	zend_declare_property_long(ToastNotificationCE, ZEND_STRL("template"), 0, ZEND_ACC_PRIVATE);
	zend_declare_property_stringl(ToastNotificationCE, ZEND_STRL("text1"), ZEND_STRL(""), ZEND_ACC_PUBLIC);
	zend_declare_property_stringl(ToastNotificationCE, ZEND_STRL("text2"), ZEND_STRL(""), ZEND_ACC_PUBLIC);
	zend_declare_property_stringl(ToastNotificationCE, ZEND_STRL("text3"), ZEND_STRL(""), ZEND_ACC_PUBLIC);

	REGISTER_LONG_CONSTANT("TOAST_NOTIFICATION_TEMPLATE_TEXT_01", TOAST_NOTIFICATION_TEMPLATE_TEXT_01, CONST_CS | CONST_PERSISTENT | CONST_CT_SUBST);
	REGISTER_LONG_CONSTANT("TOAST_NOTIFICATION_TEMPLATE_TEXT_02", TOAST_NOTIFICATION_TEMPLATE_TEXT_02, CONST_CS | CONST_PERSISTENT | CONST_CT_SUBST);
	REGISTER_LONG_CONSTANT("TOAST_NOTIFICATION_TEMPLATE_TEXT_03", TOAST_NOTIFICATION_TEMPLATE_TEXT_03, CONST_CS | CONST_PERSISTENT | CONST_CT_SUBST);
	REGISTER_LONG_CONSTANT("TOAST_NOTIFICATION_TEMPLATE_TEXT_04", TOAST_NOTIFICATION_TEMPLATE_TEXT_04, CONST_CS | CONST_PERSISTENT | CONST_CT_SUBST);
	REGISTER_LONG_CONSTANT("TOAST_NOTIFICATION_TEMPLATE_IMAGE_AND_TEXT_01", TOAST_NOTIFICATION_TEMPLATE_IMAGE_AND_TEXT_01, CONST_CS | CONST_PERSISTENT | CONST_CT_SUBST);
	REGISTER_LONG_CONSTANT("TOAST_NOTIFICATION_TEMPLATE_IMAGE_AND_TEXT_02", TOAST_NOTIFICATION_TEMPLATE_IMAGE_AND_TEXT_02, CONST_CS | CONST_PERSISTENT | CONST_CT_SUBST);
	REGISTER_LONG_CONSTANT("TOAST_NOTIFICATION_TEMPLATE_IMAGE_AND_TEXT_03", TOAST_NOTIFICATION_TEMPLATE_IMAGE_AND_TEXT_03, CONST_CS | CONST_PERSISTENT | CONST_CT_SUBST);
	REGISTER_LONG_CONSTANT("TOAST_NOTIFICATION_TEMPLATE_IMAGE_AND_TEXT_04", TOAST_NOTIFICATION_TEMPLATE_IMAGE_AND_TEXT_04, CONST_CS | CONST_PERSISTENT | CONST_CT_SUBST);

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(toastnotifications) {
	return SUCCESS;
}

PHP_RINIT_FUNCTION(toastnotifications) {
#if defined(COMPILE_DL_TOASTNOTIFICATIONS) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(toastnotifications) {
	return SUCCESS;
}

ZEND_METHOD(ToastNotification, __construct) {
	zend_long toastTemplate;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &toastTemplate) != SUCCESS) {
		return;
	}

	if (toastTemplate < TOAST_NOTIFICATION_TEMPLATE_MIN || toastTemplate > TOAST_NOTIFICATION_TEMPLATE_MAX) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Invalid template ID");
		return;
	}

	zend_update_property_long(ToastNotificationCE, getThis(), ZEND_STRL("template"), toastTemplate);
}

ZEND_METHOD(ToastNotification, display) {
	zval *toastTemplate = NULL, *property = NULL;
	ComPtr<IToastNotificationManagerStatics> toastNotificationManagerStatics;
	ComPtr<IToastNotifier> toastNotifier;
	ComPtr<IToastNotificationFactory> toastFactory;
	ComPtr<IToastNotification> toastNotification;
	ComPtr<IXmlDocument> xml;
	ComPtr<IXmlNodeList> xmlNodeList;
	ComPtr<IXmlNode> xmlNode;
	ComPtr<IXmlText> xmlText;
	HSTRING string;
	HRESULT hr;

	if (FAILED(hr = WindowsCreateString(RuntimeClass_Windows_UI_Notifications_ToastNotificationManager, wcslen(RuntimeClass_Windows_UI_Notifications_ToastNotificationManager), &string))) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to get string reference: %x", hr);
		return;
	}

	if (FAILED(hr = GetActivationFactory(string, &toastNotificationManagerStatics))) {
		WindowsDeleteString(string);
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to get instance of ToastNotificationManagerStatics: %x", hr);
		return;
	}

	WindowsDeleteString(string);

	toastTemplate = zend_read_property(ToastNotificationCE, getThis(), ZEND_STRL("template"), 0, toastTemplate);

	switch (Z_LVAL_P(toastTemplate)) {
		case TOAST_NOTIFICATION_TEMPLATE_TEXT_01:
			if (FAILED(hr = toastNotificationManagerStatics.Get()->GetTemplateContent(ToastTemplateType_ToastText01, &xml))) {
				zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to get template content: %x", hr);
				return;
			}

			if (FAILED(hr = WindowsCreateString(L"text", 4, &string))) {
				zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to get string reference: %x", hr);
				return;
			}

			if (FAILED(hr = xml->GetElementsByTagName(string, &xmlNodeList))) {
				WindowsDeleteString(string);
				zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to get XML node list: %x", hr);
				return;
			}

			WindowsDeleteString(string);

			if (FAILED(hr = xmlNodeList->Item(0, &xmlNode))) {
				zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to get XML node: %x", hr);
				return;
			}

			property = zend_read_property(ToastNotificationCE, getThis(), ZEND_STRL("text1"), 0, property);

			if (Z_STRLEN_P(property)) {
				unsigned int wideLength = MultiByteToWideChar(CP_UTF8, 0, Z_STRVAL_P(property), Z_STRLEN_P(property) + 1, NULL, 0);
				wchar_t *wide = (wchar_t*) emalloc(wideLength * sizeof(wchar_t));
				ComPtr<IXmlNode> stringNode;
				ComPtr<IXmlNode> childNode;

				if (!MultiByteToWideChar(CP_UTF8, 0, Z_STRVAL_P(property), Z_STRLEN_P(property) + 1, wide, wideLength)) {
					zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to convert string: %x", GetLastError());
					return;
				}

				if (FAILED(hr = WindowsCreateString(wide, wideLength - 1, &string))) {
					zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to get string reference: %x", hr);
					return;
				}

				if (FAILED(hr = xml->CreateTextNode(string, &xmlText))) {
					WindowsDeleteString(string);
					zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to create XML text: %x", hr);
					return;
				}

				WindowsDeleteString(string);

				if (FAILED(hr = xmlText.As(&stringNode))) {
					zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to create XML text node: %x", hr);
					return;
				}

				if (FAILED(hr = xmlNode->AppendChild(stringNode.Get(), &childNode))) {
					zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to append XML text node: %x", hr);
					return;
				}
			}

			break;
	}

	if (FAILED(hr = WindowsCreateString(L"php", 3, &string))) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to get string reference: %x", hr);
		return;
	}

	if (FAILED(hr = toastNotificationManagerStatics->CreateToastNotifierWithId(string, &toastNotifier))) {
		WindowsDeleteString(string);
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to create toast notifier: %x", hr);
		return;
	}

	WindowsDeleteString(string);

	if (FAILED(hr = WindowsCreateString(RuntimeClass_Windows_UI_Notifications_ToastNotification, wcslen(RuntimeClass_Windows_UI_Notifications_ToastNotification), &string))) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to get string reference: %x", hr);
		return;
	}

	if (FAILED(hr = GetActivationFactory(string, &toastFactory))) {
		WindowsDeleteString(string);
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to get instance of ToastNotificationFactory: %x", hr);
		return;
	}

	WindowsDeleteString(string);

	if (FAILED(hr = toastFactory->CreateToastNotification(xml.Get(), &toastNotification))) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to create toast notification: %x", hr);
		return;
	}

	if (FAILED(hr = toastNotifier->Show(toastNotification.Get()))) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to show toast notification: %x", hr);
		return;
	}
}

zend_module_entry toastnotifications_module_entry = {
	STANDARD_MODULE_HEADER,
	"toastnotifications",
	NULL,
	PHP_MINIT(toastnotifications),
	PHP_MSHUTDOWN(toastnotifications),
	PHP_RINIT(toastnotifications),
	PHP_RSHUTDOWN(toastnotifications),
	NULL,
	"1.0",
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_TOASTNOTIFICATIONS
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(toastnotifications)
#endif
