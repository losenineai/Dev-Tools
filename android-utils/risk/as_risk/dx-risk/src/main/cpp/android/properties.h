/*
 * properties.h
 *
 *  Created on: 2016年12月14日
 *      Author: boyliang
 */

#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include <string>

using namespace std;

/* property_get: returns the length of the value which will never be
** greater than PROPERTY_VALUE_MAX - 1 and will always be zero terminated.
** (the length does not include the terminating zero).
**
** If the property read fails or returns an empty value, the default
** value is used (if nonnull).
*/
int property_get(const char *key, char *value, const char *default_value);

/* property_set: returns 0 on success, < 0 on failure
*/
int property_set(const char *key, const char *value);

string property_get_ex(const string &key);


#endif /* PROPERTIES_H_ */
