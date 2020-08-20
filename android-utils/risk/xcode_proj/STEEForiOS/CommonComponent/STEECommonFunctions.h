//
//  STEECommonFunctions.h
//  STEEForiOS
//
//  Created by echonn on 02/07/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#ifndef STEECommonFunctions_h
#define STEECommonFunctions_h

#define CString2NSString(cstring) (cstring == NULL ? @"" : [NSString stringWithCString:cstring encoding:NSUTF8StringEncoding])
#define CPPString2NSString(cppstring) CString2NSString(cppstring.c_str())

#define NSString2CString(nsstring) (nsstring == nil ? [@"" cStringUsingEncoding:NSUTF8StringEncoding] : [nsstring cStringUsingEncoding:NSUTF8StringEncoding])

#define NSString2CPPString(nsstring) (nsstring == nil ? string("") : string([nsstring cStringUsingEncoding:NSUTF8StringEncoding]))
#define NSData2CPPString(nsdata) (nsdata == nil ? string("") : string((char *)[nsdata bytes], [nsdata length]))

#define CPPString2NSData(cppstring) (cppstring == string("") ? nil : [NSData dataWithBytes:(const void *)(cppstring.c_str()) length: cppstring.length()]);

#endif /* STEECommonFunctions_h */
