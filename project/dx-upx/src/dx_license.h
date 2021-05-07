//
// Created by dongyang on 18-4-24.
//

#ifndef DX_LICENSE_DX_LICENSE_H
#define DX_LICENSE_DX_LICENSE_H

#include "failure_cause.h"

// typedef void *stee_license_t;

#define KEEP_SYM __attribute__((__annotate__(("keep-symbol"))))

#define dxlic_init                              iIgIlI0gO1iLI1O0I00ogg0igiLlIgIOO
#define dxlic_release                           iOiiiOIOl0IiIl1oOI90gIoLOllll9IOI
#define dxlic_setLicenseFile                    iligLILgLigoO1OlliIo0I19L90i9iO10
#define dxlic_setLicense                        iIlIOLioLlo019g0OL9iL1io1OogLiOl1
#define dxlic_checkProductLicenseValid          igoIIi0iiOo0g9OoO9O9IlglLggI919lo
#define dxlic_checkLicenseValid                 ig09Iiii00ligo9g0L9Lo9oIiLggli0O1
#define dxlic_getProductExpreidTime             iiLIlO0giILILO9i10Li009o1l91Lgigo
#define dxlic_getExpreidTime                    ii90lII00ol0Oi9O0O0ILgilIiioliggi
#define dxlic_getUser                           iOLI1il90g1ogi1OI1oI9LIiIL9oO9oIL
#define dxlic_checkProductFeature               ii10igLIOL1Llo1g9llgIll9LI9Lg1IOL
#define dxlic_checkFeature                      io0g1Loo1iOo0gOIgloLLoiig99iL1lLg
#define dxlic_stee_license_get_package_name     iL0og1io1L1io0Io10igi9Li1I9LgIo1g
#define dxlic_stee_license_get_obf_limit        iLiO9L0O009oO0g1IllgIloOOOIg0ggg0
#define dxlic_stee_license_get_xse_limit        ig1L9o1OiI90g1ggllIlI1IO0ioOIO0i9
#define dxlic_stee_license_get_xsc_limit        iIL0ililol01gO1I0lggiIgiL0oIg09Il
#define dxlic_stee_license_get_vmp_limit        igl000l1OOioIlOio1L9ol01ig0igII1I
#define dxlic_getCause                          igl000l1OOioIlOio1L9o101ig0igII1I

#ifdef __cplusplus
extern "C" {
#endif


KEEP_SYM void dxlic_init();
KEEP_SYM void dxlic_release();
KEEP_SYM int dxlic_setLicenseFile(const char *fileName);
KEEP_SYM int dxlic_setLicense(const char *licenseData, int length);
KEEP_SYM int dxlic_checkProductLicenseValid(const char *productName);
KEEP_SYM int dxlic_checkLicenseValid();
KEEP_SYM const char *dxlic_getProductExpreidTime(const char *productName);
KEEP_SYM const char *dxlic_getExpreidTime();
KEEP_SYM const char *dxlic_getUser();
KEEP_SYM int dxlic_checkProductFeature(const char *productName, const char *featureName);
KEEP_SYM int dxlic_checkFeature(const char *featureName);
KEEP_SYM enum FAILURE_CAUSE dxlic_getCause();
KEEP_SYM const char* dxlic_stee_license_get_package_name();
KEEP_SYM int dxlic_stee_license_get_obf_limit();
KEEP_SYM int dxlic_stee_license_get_xse_limit();
KEEP_SYM int dxlic_stee_license_get_xsc_limit();
KEEP_SYM int dxlic_stee_license_get_vmp_limit();

#ifdef __cplusplus
}
#endif

#endif //DX_LICENSE_DX_LICENSE_H