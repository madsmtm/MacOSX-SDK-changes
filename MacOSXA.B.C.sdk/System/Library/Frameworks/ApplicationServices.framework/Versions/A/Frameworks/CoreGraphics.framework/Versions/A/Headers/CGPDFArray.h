/* CoreGraphics - CGPDFArray.h
 * Copyright (c) 2002-2003 Apple Computer, Inc. (unpublished)
 * All rights reserved.
 */

#ifndef CGPDFARRAY_H_
#define CGPDFARRAY_H_

typedef struct CGPDFArray *CGPDFArrayRef;

#include <CoreGraphics/CGPDFDictionary.h>
#include <CoreGraphics/CGPDFObject.h>
#include <CoreGraphics/CGPDFStream.h>
#include <CoreGraphics/CGPDFString.h>

CG_EXTERN_C_BEGIN

/* Return the number of items in `array'. */

CG_EXTERN size_t CGPDFArrayGetCount(CGPDFArrayRef array);

/* Look up the object at `index' in `array' and return the result in
 * `value'. Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetObject(CGPDFArrayRef array, size_t index, CGPDFObjectRef *value);

/* Look up the object at `index' in `array' and, if it's a null, return
 * true; otherwise, return false. */

CG_EXTERN bool CGPDFArrayGetNull(CGPDFArrayRef array, size_t index);

/* Look up the object at `index' in `array' and, if it's a boolean, return
 * the result in `value'.  Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetBoolean(CGPDFArrayRef array, size_t index, CGPDFBoolean *value);

/* Look up the object at `index' in `array' and, if it's an integer, return
 * the result in `value'.  Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetInteger(CGPDFArrayRef array, size_t index, CGPDFInteger *value);

/* Look up the object at `index' in `array' and, if it's a number (real or
 * integer), return the result in `value'.  Return true on success; false
 * otherwise. */

CG_EXTERN bool CGPDFArrayGetNumber(CGPDFArrayRef array, size_t index, CGPDFReal *value);

/* Look up the object at `index' in `array' and, if it's a name, return the
 * result in `value'.  Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetName(CGPDFArrayRef array, size_t index, const char **value);

/* Look up the object at `index' in `array' and, if it's a string, return
 * the result in `value'.  Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetString(CGPDFArrayRef array, size_t index, CGPDFStringRef *value);

/* Look up the object at `index' in `array' and, if it's an array, return
 * it in `value'.  Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetArray(CGPDFArrayRef array, size_t index, CGPDFArrayRef *value);

/* Look up the object at `index' in `array' and, if it's a dictionary,
 * return it in `value'.  Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetDictionary(CGPDFArrayRef array, size_t index, CGPDFDictionaryRef *value);

/* Look up the object at `index' in `array' and, if it's a stream, return
 * it in `value'.  Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetStream(CGPDFArrayRef array, size_t index, CGPDFStreamRef *value);

CG_EXTERN_C_END

#endif	/* CGPDFARRAY_H_ */
