/*	CFPlugIn.h
	Copyright (c) 1999-2003, Apple, Inc. All rights reserved.
*/

#if !defined(__COREFOUNDATION_CFPLUGIN__)
#define __COREFOUNDATION_CFPLUGIN__ 1

#if !defined(COREFOUNDATION_CFPLUGINCOM_SEPARATE)
#define COREFOUNDATION_CFPLUGINCOM_SEPARATE 1
#endif

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFArray.h>
#include <CoreFoundation/CFBundle.h>
#include <CoreFoundation/CFString.h>
#include <CoreFoundation/CFURL.h>
#include <CoreFoundation/CFUUID.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* ================ Standard Info.plist keys for plugIns ================ */

CF_EXPORT
const CFStringRef kCFPlugInDynamicRegistrationKey;
CF_EXPORT
const CFStringRef kCFPlugInDynamicRegisterFunctionKey;
CF_EXPORT
const CFStringRef kCFPlugInUnloadFunctionKey;
CF_EXPORT
const CFStringRef kCFPlugInFactoriesKey;
CF_EXPORT
const CFStringRef kCFPlugInTypesKey;

/* ================= Function prototypes for various callbacks ================= */
/* Function types that plugIn authors can implement for various purposes. */

typedef void (*CFPlugInDynamicRegisterFunction)(CFPlugInRef plugIn);
typedef void (*CFPlugInUnloadFunction)(CFPlugInRef plugIn);
typedef void *(*CFPlugInFactoryFunction)(CFAllocatorRef allocator, CFUUIDRef typeUUID);

/* ================= Creating PlugIns ================= */

CF_EXPORT
UInt32 CFPlugInGetTypeID(void);

CF_EXPORT
CFPlugInRef CFPlugInCreate(CFAllocatorRef allocator, CFURLRef plugInURL);
    /* Might return an existing instance with the ref-count bumped. */

CF_EXPORT
CFBundleRef CFPlugInGetBundle(CFPlugInRef plugIn);

/* ================= Controlling load on demand ================= */
/* For plugIns. */
/* PlugIns that do static registration are load on demand by default. */
/* PlugIns that do dynamic registration are not load on demand by default. */
/* A dynamic registration function can call CFPlugInSetLoadOnDemand(). */

CF_EXPORT
void CFPlugInSetLoadOnDemand(CFPlugInRef plugIn, Boolean flag);

CF_EXPORT
Boolean CFPlugInIsLoadOnDemand(CFPlugInRef plugIn);

/* ================= Finding factories and creating instances ================= */
/* For plugIn hosts. */
/* Functions for finding factories to create specific types and actually creating instances of a type. */

CF_EXPORT
CFArrayRef CFPlugInFindFactoriesForPlugInType(CFUUIDRef typeUUID);
    /* This function finds all the factories from any plugin for the given type.  Returns an array that the caller must release. */
    
CF_EXPORT
CFArrayRef CFPlugInFindFactoriesForPlugInTypeInPlugIn(CFUUIDRef typeUUID, CFPlugInRef plugIn);
    /* This function restricts the result to factories from the given plug-in that can create the given type.  Returns an array that the caller must release. */

CF_EXPORT
void *CFPlugInInstanceCreate(CFAllocatorRef allocator, CFUUIDRef factoryUUID, CFUUIDRef typeUUID);
    /* This function returns the IUnknown interface for the new instance. */

/* ================= Registering factories and types ================= */
/* For plugIn writers who must dynamically register things. */
/* Functions to register factory functions and to associate factories with types. */

CF_EXPORT
Boolean CFPlugInRegisterFactoryFunction(CFUUIDRef factoryUUID, CFPlugInFactoryFunction func);

CF_EXPORT
Boolean CFPlugInRegisterFactoryFunctionByName(CFUUIDRef factoryUUID, CFPlugInRef plugIn, CFStringRef functionName);

CF_EXPORT
Boolean CFPlugInUnregisterFactory(CFUUIDRef factoryUUID);

CF_EXPORT
Boolean CFPlugInRegisterPlugInType(CFUUIDRef factoryUUID, CFUUIDRef typeUUID);

CF_EXPORT
Boolean CFPlugInUnregisterPlugInType(CFUUIDRef factoryUUID, CFUUIDRef typeUUID);

/* ================= Registering instances ================= */
/* When a new instance of a type is created, the instance is responsible for registering itself with the factory that created it and unregistering when it deallocates. */
/* This means that an instance must keep track of the CFUUIDRef of the factory that created it so it can unregister when it goes away. */

CF_EXPORT
void CFPlugInAddInstanceForFactory(CFUUIDRef factoryID);

CF_EXPORT
void CFPlugInRemoveInstanceForFactory(CFUUIDRef factoryID);


/* Obsolete API */

typedef struct __CFPlugInInstance *CFPlugInInstanceRef;

typedef Boolean (*CFPlugInInstanceGetInterfaceFunction)(CFPlugInInstanceRef instance, CFStringRef interfaceName, void **ftbl);
typedef void (*CFPlugInInstanceDeallocateInstanceDataFunction)(void *instanceData);

CF_EXPORT
Boolean CFPlugInInstanceGetInterfaceFunctionTable(CFPlugInInstanceRef instance, CFStringRef interfaceName, void **ftbl);
CF_EXPORT
CFStringRef CFPlugInInstanceGetFactoryName(CFPlugInInstanceRef instance);
CF_EXPORT
void *CFPlugInInstanceGetInstanceData(CFPlugInInstanceRef instance);
CF_EXPORT
UInt32 CFPlugInInstanceGetTypeID(void);
CF_EXPORT
CFPlugInInstanceRef CFPlugInInstanceCreateWithInstanceDataSize(CFAllocatorRef allocator, CFIndex instanceDataSize, CFPlugInInstanceDeallocateInstanceDataFunction deallocateInstanceFunction, CFStringRef factoryName, CFPlugInInstanceGetInterfaceFunction getInterfaceFunction);

#if defined(__cplusplus)
}
#endif

#if !COREFOUNDATION_CFPLUGINCOM_SEPARATE
#include <CoreFoundation/CFPlugInCOM.h>
#endif

#endif /* ! __COREFOUNDATION_CFPLUGIN__ */

