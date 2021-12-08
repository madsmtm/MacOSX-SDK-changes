/*
     File:       OT/OpenTransport.h
 
     Contains:   Open Transport client interface file.
 
     Version:    OpenTransport-110~351
 
     Copyright:  � 1985-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __OPENTRANSPORT__
#define __OPENTRANSPORT__





/*
    All OpenTransport Manager APIs are deprecated in MacOSX 10.4, instead of using OpenTransport,
    consider using CFNetwork or socket library.
*/
/*
   The following table shows how to map from the old (pre-Universal
   Interfaces) header file name to the equivalent Universal Interfaces
   header file name.
    Old Header              New Header
    ----------              ----------
    cred.h                  OpenTransportProtocol.h
    dlpi.h                  OpenTransportProtocol.h
    miioccom.h              OpenTransportProtocol.h
    mistream.h              OpenTransportProtocol.h/OpenTransportKernel.h
    modnames.h              OpenTransportProtocol.h
    OpenTptAppleTalk.h      OpenTransportProviders.h
    OpenTptClient.h         OpenTransportProtocol.h
    OpenTptCommon.h         OpenTransportProtocol.h
    OpenTptConfig.h         OpenTransportProtocol.h
    OpenTptDevLinks.h       OpenTransportProviders.h
    OpenTptInternet.h       OpenTransportProviders.h
    OpenTptISDN.h           OpenTransportProviders.h
    OpenTptLinks.h          OpenTransportProviders.h
    OpenTptModule.h         OpenTransportKernel.h
    OpenTptPCISupport.h     OpenTransportKernel.h
    OpenTptSerial.h         OpenTransportProviders.h
    OpenTptXTI.h            OpenTransportUNIX.r
    OpenTransport.h         OpenTransport.h
    OpenTransport.r         OpenTransport.r
    OTConfig.r              OpenTransportProtocol.r
    OTDebug.h               OpenTransport.h
    OTSharedLibs.h          OpenTransportProviders.h
    strlog.h                OpenTransportProtocol.h/OpenTransportKernel.h
    stropts.h               OpenTransportProtocol.h/OpenTransportUNIX.h
    strstat.h               OpenTransportProtocol.h
    tihdr.h                 OpenTransportProtocol.h
*/

#ifndef __MACTYPES__
#include <CarbonCore/MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <CarbonCore/MixedMode.h>
#endif

#ifndef __MACERRORS__
#include <CarbonCore/MacErrors.h>
#endif

#ifdef __cplusplus
#include <stddef.h>
#endif

/* #include <OTEnvironment.h> */

#define _CDECL

#if defined(__SC__) || defined(THINK_CPLUS) || defined(__MRC__)
   #ifdef __cplusplus
     #undef _CDECL
      #define _CDECL          _cdecl
 #endif
#endif


#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)


#if defined(__MWERKS__) && TARGET_CPU_68K
 #pragma push
   #pragma pointers_in_D0
#endif

/* ***** Setup Default Compiler Variables ******/

/*
   OTKERNEL is used to indicate whether the code is being built
   for the kernel environment.  It defaults to 0.  If you include
   "OpenTransportKernel.h" before including this file,
   it will be 1 and you will only be able to see stuff available
   to kernel code.
*/

#ifndef OTKERNEL
#define OTKERNEL 0
#endif  /* !defined(OTKERNEL) */

/*
   OTUNIXERRORS determines whether this file defines a bunch of
   common UNIX error codes, like EPERM.  Typically, client code does
   not want to do this because of the possibility of a clash with
   other code modules, like the standard C libraries, that also
   defines these routines.  However, client code can turn it on to
   get these definitions.  This might be done by protocol stack
   infrastructure, or some other low-level code.
   "OpenTransportKernel.i" sets this flag before include
   "OpenTransport.h" because kernel modules typically need these
   error codes.  Note that kernel modules shouldn't be including
   standard C libraries, so this is rarely a problem.
   In general, the clash between OT and standard C definitions
   of these error codes is rarely a problem becasue both OT
   and the C libraries define them to have the same value.  But
   I'm sure this check is useful to some people.
*/
#ifndef OTUNIXERRORS
#define OTUNIXERRORS 0
#endif  /* !defined(OTUNIXERRORS) */

/*
   OTDEBUG is used to control the behaviour of the OT debugging
   macros.  If you set it to non-zero, the macros will generate code
   that drops you into the debugger.  If you set it to 0, or leave it
   undefined, the macros are compiled out.
   Setting up this compiler variable is a little tricky because previous
   versions of the OT interfaces used a different variable, qDebug.
   We replaced qDebug with OTDEBUG because qDebug does not fit into
   the OT namespace.  But I didn't want to break a lot of currently
   building code.  The following tricky compiler variable footwork
   avoids this.
   There are four outcomes when this code is compiled, depending on
   whether qDebug and OTDEBUG are defined beforehand.  The following
   table shows the outcome in each case.
   qDebug     OTDEBUG    Outcome       Explanation  
   ------     -------    -------       -----------
   defined    defined    OTDEBUG wins  Mixed legacy and new code, we believe the new code.
   defined    undefined  qDebug wins   Legacy code.
   undefined  defined    OTDEBUG wins  New code.
   undefined  undefined  no debugging  No debugging.
*/
#ifdef qDebug
#ifndef OTDEBUG
#define OTDebug qDebug
#endif  /* !defined(OTDEBUG) */

#endif  /* defined(qDebug) */

#ifndef OTDEBUG
#define OTDEBUG 0
#endif  /* !defined(OTDEBUG) */

#ifndef BuildingOTForKernel
#define BuildingOTForKernel 0
#endif  /* !defined(BuildingOTForKernel) */

/* Carbon Applications have some restrictions on using OT*/
#ifndef OTCARBONAPPLICATION
#define OTCARBONAPPLICATION 0
#endif  /* !defined(OTCARBONAPPLICATION) */

/*
   ***** Normalise 68K Calling C Conventions *****
   Define special types that handle the difference in parameter passing
   between different Mac OS C compilers when generating 68K code.  OT
   exports C calling conventions routines, and various C compilers use
   various different conventions.  Differences in the placement of the result
   are covered above, where we output pragma pointers_in_D0.  The other big
   difference is how the compilers pass integer parameters less than 32 bits.
   The MPW compiler always extends these to 32 bits; other compilers simply
   push a value of the appropriate size.  We overcome this difference by
   defining special OTFooParam types, which are only used when passing
   sub 32 bit values to routines.  They are always defined to a 32 bit
   size, which makes all the compilers do the same thing.
   One weird consequence of this is that in more strict type checking
   languages (eg Pascal) OTBooleanParam is not compatible with Boolean.
   Sorry.
*/



/*
   Large tracts of OT source still uses boolean_p etc.
   So we continue to define the old types for Apple
   clients.  The long term fix is to remove all the
   uses of these type from the OT source, but that's
   beyond the scope of my work right now.
*/
typedef UInt8                           OTUInt8Param;
typedef UInt16                          OTUInt16Param;
typedef SInt16                          OTSInt16Param;
typedef SInt8                           OTSInt8Param;
typedef Boolean                         OTBooleanParam;
typedef UInt8                           uchar_p;
typedef UInt16                          ushort_p;
typedef SInt16                          short_p;
typedef SInt8                           char_p;
typedef Boolean                         boolean_p;

#if defined(__MWERKS__) || defined(THINK_C) || defined(SYMANTEC_CPLUS) || defined(SYMANTEC_C) || defined (__xlc) || defined (__xlC) || defined (__xlC__)
typedef ByteCount                       OTByteCount;
typedef ItemCount                       OTItemCount;
typedef SInt32                          OTInt32;
typedef UInt32                          OTUInt32;
#else
    typedef unsigned int    OTByteCount;
    typedef unsigned int    OTItemCount;
    typedef int             OTInt32;
    typedef unsigned int    OTUInt32;
#endif

typedef long                            int_t;
typedef unsigned long                   uint_t;
/* ***** C++ Support ******/

/*
   Setup _MDECL to be _cdecl when compiling C++ code with
   compilers that support it, or nothing otherwise.
*/



#define _MDECL
#if defined(__SC__) || defined(THINK_CPLUS) || defined(__MRC__)
    #ifdef __cplusplus
     #undef _MDECL
      #define _MDECL          _cdecl
 #endif
#endif

/* ***** Shared Library Prefixes ******/


#define kOTLibraryVersion      "1.1"

#define kOTLibraryPrefix         "OTLib$"
#define kOTModulePrefix            "OTModl$"
#define kOTClientPrefix           "OTClnt$"
#define kOTKernelPrefix           "OTKrnl$"

enum {
  kOTCFMClass                   = 'otan'
};

/* ***** Miscellaneous Type Definitions ******/

/* A millisecond timeout value*/
typedef UInt32                          OTTimeout;
/* An ID number in connections/transactions     */
typedef SInt32                          OTSequence;
/* An ID number for registered names            */
typedef SInt32                          OTNameID;
/*
   A protocol-specific reason code for failure.
   Usually a Unix-style positive error code.
*/
typedef SInt32                          OTReason;
/* Number of outstanding connection requests at a time.*/
typedef UInt32                          OTQLen;
/* Will become internationalizeable shortly (yeah, right).*/
typedef UInt8 *                         OTClientName;
/* The command code in STREAMS messages.*/
typedef SInt32                          OTCommand;
/* value describing a client*/
typedef void* OTClient;

/*
    OT now defines its own version of the standard C "offsetof"
    macro so as to avoid including <stddef.h>.
*/
#define OTOffsetOf(structure,field) ((ByteCount)&((structure *) 0)->field)

/* ***** Debugging Macros ******/


#define kOTFatalErr                "FB "
#define kOTNonfatalErr            "NB "
#define kOTExtFatalErr            "FX "
#define kOTExtNonfatalErr     "NX "
#define kOTUserFatalErr           "UF "
#define kOTUserErr                "UE "
#define kOTUserNonfatalErr        "UE "
#define kOTInfoErr                "IE "
#define kOTInfoBreak          "IN "


/*
 *  OTDebugStr()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */



#if qDebug > 0
    #define OTDebugBreak(str)           OTDebugStr(str)
    #define OTDebugTest(val, str)       { if ( val ) OTDebugStr(str); }
    #define OTAssert(name, cond)        ((cond) ? ((void) 0) : (OTDebugStr( __FILE__ ": " #name ": " #cond )))
#else
   #define OTDebugBreak(str)
  #define OTDebugTest(val, str)
  #define OTAssert(name, cond)
#endif /* qDebug > 0 */

#if qDebug > 1 || qDebug2 > 1
    #define OTDebugBreak2(str)          OTDebugStr(str)
    #define OTDebugTest2(val, str)      { if ( val) OTDebugStr(str); }
#else
   #define OTDebugBreak2(str)
 #define OTDebugTest2(val, str)
#endif   /* qDebug > 1 || qDebug2 > 1 */

/*
   ***** Flags Used When Opening Providers *****
   Important
   OT does not currently support any of these flags.  You should
   always pass 0 to a parameter of type OTOpenFlags.  If you need
   to modify the mode of operation of a provider, use OTSetBlocking,
   OTSetSynchronous, etc.
*/
typedef UInt32 OTOpenFlags;
enum {
  kO_ASYNC                      = 0x01,
  kO_NDELAY                     = 0x04,
  kO_NONBLOCK                   = 0x04
};


#if CALL_NOT_IN_CARBON
/*
   BSD defines O_ASYNC, O_NDELAY and O_NONBLOCK in fcntl.h 
   Use kO_ASYNC, kO_NDELAY and kO_NONBLOCK in the unlikely event you need the OT value in Carbon
*/
enum {
  O_ASYNC                       = kO_ASYNC,
  O_NDELAY                      = kO_NDELAY,
  O_NONBLOCK                    = kO_NONBLOCK
};

#endif  /* CALL_NOT_IN_CARBON */

/* ***** UNIX-Style Error Codes ******/

typedef UInt16                          OTUnixErr;
/*
   These definitions are only compiled if you're building kernel code
   or you explicit request them by setting OTUNIXERRORS.  See the
   description of these compiler variables, given above.
*/
#if OTKERNEL || OTUNIXERRORS
/*
   There may be some error code confusions with other compiler vendor header
   files - However, these match both MPW and AIX definitions.
*/
/*
   First we undefine the #defined ones we know about so that we can put them
   in an enum.  Of course, this is only going to work in C, but hopefully
   other languages won't have these symbols overloaded.
*/

     
  #undef EPERM
   #undef ENOENT
  #undef ENORSRC
 #undef EINTR
   #undef EIO
 #undef ENXIO
   #undef E2BIG
   #undef EBADF
   #undef EAGAIN
  #undef ENOMEM
  #undef EACCES
  #undef EFAULT
  #undef EBUSY
   #undef EEXIST
  #undef ENODEV
  #undef EINVAL
  #undef ENOTTY
  #undef ERANGE
  #undef ESRCH
   #undef EPIPE

enum {
  EPERM                         = 1,    /* Permission denied            */
  ENOENT                        = 2,    /* No such file or directory       */
  ENORSRC                       = 3,    /* No such resource               */
  EINTR                         = 4,    /* Interrupted system service        */
  EIO                           = 5,    /* I/O error                 */
  ENXIO                         = 6,    /* No such device or address       */
  EBADF                         = 9,    /* Bad file number                 */
  EAGAIN                        = 11,   /* Try operation again later       */
  ENOMEM                        = 12,   /* Not enough space               */
  EACCES                        = 13,   /* Permission denied            */
  EFAULT                        = 14,   /* Bad address                   */
  EBUSY                         = 16,   /* Device or resource busy          */
  EEXIST                        = 17,   /* File exists                   */
  ENODEV                        = 19,   /* No such device               */
  EINVAL                        = 22,   /* Invalid argument               */
  ENOTTY                        = 25,   /* Not a character device          */
  EPIPE                         = 32,   /* Broken pipe                   */
  ERANGE                        = 34,   /* Math result not representable   */
  EDEADLK                       = 35,   /* Call would block so was aborted       */
  EWOULDBLOCK                   = 35,   /* Or a deadlock would occur       */
  EALREADY                      = 37,
  ENOTSOCK                      = 38,   /* Socket operation on non-socket     */
  EDESTADDRREQ                  = 39,   /* Destination address required      */
  EMSGSIZE                      = 40,   /* Message too long               */
  EPROTOTYPE                    = 41,   /* Protocol wrong type for socket     */
  ENOPROTOOPT                   = 42,   /* Protocol not available          */
  EPROTONOSUPPORT               = 43,   /* Protocol not supported          */
  ESOCKTNOSUPPORT               = 44,   /* Socket type not supported       */
  EOPNOTSUPP                    = 45,   /* Operation not supported on socket  */
  EADDRINUSE                    = 48,   /* Address already in use          */
  EADDRNOTAVAIL                 = 49,   /* Can't assign requested address     */
  ENETDOWN                      = 50,   /* Network is down                 */
  ENETUNREACH                   = 51,   /* Network is unreachable          */
  ENETRESET                     = 52,   /* Network dropped connection on reset    */
  ECONNABORTED                  = 53,   /* Software caused connection abort     */
  ECONNRESET                    = 54,   /* Connection reset by peer          */
  ENOBUFS                       = 55,   /* No buffer space available       */
  EISCONN                       = 56,   /* Socket is already connected         */
  ENOTCONN                      = 57,   /* Socket is not connected          */
  ESHUTDOWN                     = 58,   /* Can't send after socket shutdown     */
  ETOOMANYREFS                  = 59,   /* Too many references: can't splice  */
  ETIMEDOUT                     = 60,   /* Connection timed out             */
  ECONNREFUSED                  = 61,   /* Connection refused           */
  EHOSTDOWN                     = 64,   /* Host is down                */
  EHOSTUNREACH                  = 65,   /* No route to host               */
  EPROTO                        = 70,   /* STREAMS protocol error          */
  ETIME                         = 71,
  ENOSR                         = 72,
  EBADMSG                       = 73,
  ECANCEL                       = 74,
  ENOSTR                        = 75,
  ENODATA                       = 76,
  EINPROGRESS                   = 77,
  ESRCH                         = 78,
  ENOMSG                        = 79,
  ELASTERRNO                    = 79
};

#endif  /* OTKERNEL || OTUNIXERRORS */

/* ***** Open Transport/XTI Error codes ******/
typedef UInt16                          OTXTIErr;
enum {
  TSUCCESS                      = 0,    /* No Error occurred             */
  TBADADDR                      = 1,    /* A Bad address was specified          */
  TBADOPT                       = 2,    /* A Bad option was specified          */
  TACCES                        = 3,    /* Missing access permission          */
  TBADF                         = 4,    /* Bad provider reference           */
  TNOADDR                       = 5,    /* No address was specified             */
  TOUTSTATE                     = 6,    /* Call issued in wrong state          */
  TBADSEQ                       = 7,    /* Sequence specified does not exist   */
  TSYSERR                       = 8,    /* A system error occurred              */
  TLOOK                         = 9,    /* An event occurred - call Look()         */
  TBADDATA                      = 10,   /* An illegal amount of data was specified    */
  TBUFOVFLW                     = 11,   /* Passed buffer not big enough          */
  TFLOW                         = 12,   /* Provider is flow-controlled          */
  TNODATA                       = 13,   /* No data available for reading       */
  TNODIS                        = 14,   /* No disconnect indication available     */
  TNOUDERR                      = 15,   /* No Unit Data Error indication available    */
  TBADFLAG                      = 16,   /* A Bad flag value was supplied       */
  TNOREL                        = 17,   /* No orderly release indication available    */
  TNOTSUPPORT                   = 18,   /* Command is not supported             */
  TSTATECHNG                    = 19,   /* State is changing - try again later       */
  TNOSTRUCTYPE                  = 20,   /* Bad structure type requested for OTAlloc   */
  TBADNAME                      = 21,   /* A bad endpoint name was supplied      */
  TBADQLEN                      = 22,   /* A Bind to an in-use address with qlen > 0*/
  TADDRBUSY                     = 23,   /* Address requested is already in use       */
  TINDOUT                       = 24,   /* Accept failed because of pending listen    */
  TPROVMISMATCH                 = 25,   /* Tried to accept on incompatible endpoint   */
  TRESQLEN                      = 26,
  TRESADDR                      = 27,
  TQFULL                        = 28,
  TPROTO                        = 29,   /* An unspecified provider error occurred */
  TBADSYNC                      = 30,   /* A synchronous call at interrupt time     */
  TCANCELED                     = 31,   /* The command was cancelled          */
  TLASTXTIERROR                 = 31
};

/*
   ***** Mac OS Error Codes *****
   Most OT client routines return an OSStatus error code, a 32 bit type
   defined in "MacTypes.h".  The OT-unique error code values are
   defined below.  Many of these are generated by remapping XTI error
   codes (Txxxx) and UNIX error codes (Exxxx) to a reserved range
   in the OSStatus space.
   Some routines return an OTResult type, indicating
   that the routine might fail with a negative error, succeed with noErr,
   or possible return a positive value indicating some status.
*/

typedef SInt32                          OTResult;

/*
 * These map the Open Transport/XTI errors (the Txxxx error codes), and the
 * StdCLib Exxxx error codes into unique spaces in the Mac OS OSStatus space.
 */
#define XTI2OSStatus(x)           (-3149 - (x))
#define E2OSStatus(x)         (-3199 - (x))

#define OSStatus2XTI(x)          ((OTXTIErr)(-3149 - (x)))
#define OSStatus2E(x)         ((OTUnixErr)(-3199 - (x)))

#define IsXTIError(x)           ((x) < -3149 && (x) >= (-3149 - TLASTXTIERROR))
#define IsEError(x)             ((x) < -3199 && (x) >= (-3199 - ELASTERRNO))

/* ***** OTAddress ******/

/*
   OTAddress type defines the standard header for all OT address formats.
   It consists of one 16 bit integer, which defines the address format
   used, followed by an arbitrary number of bytes which are protocol-specific.
   Conceptually, all OT address formats are subtypes of this type,
   extended with fields that are specific to the protocol.  For example,
   OTInetAddress starts with the OTAddressType field and then continues
   to include a host IP address and a port number.
*/

enum {
  kOTGenericName                = 0     /* Protocol specific data is just a string, interpreted in a protocol-specific fashion.*/
};

typedef UInt16                          OTAddressType;
struct OTAddress {
  OTAddressType       fAddressType;           /* The address format of this address...*/
  UInt8               fAddress[1];            /* ... followed by protocol specific address information.*/
};
typedef struct OTAddress                OTAddress;
/*
   ***** OTAlloc Constants *****
   Note:
   In general, Apple recommends that you avoid the OTAlloc call because
   using it extensively causes your program to allocate and deallocate
   many memory blocks, with each extra memory allocation costing time.
*/
/*
   OTStructType defines the structure type to be allocated using the OTAlloc
   call.
*/
enum {
  T_BIND                        = 1,
  T_OPTMGMT                     = 2,
  T_CALL                        = 3,
  T_DIS                         = 4,
  T_UNITDATA                    = 5,
  T_UDERROR                     = 6,
  T_INFO                        = 7,
  T_REPLYDATA                   = 8,
  T_REQUESTDATA                 = 9,
  T_UNITREQUEST                 = 10,
  T_UNITREPLY                   = 11
};


typedef UInt32                          OTStructType;
/*
   These values are used in the "fields" parameter of the OTAlloc call
   to define which fields of the structure should be allocated.
*/
enum {
  T_ADDR                        = 0x01,
  T_OPT                         = 0x02,
  T_UDATA                       = 0x04,
  T_ALL                         = 0xFFFF
};

typedef UInt32                          OTFieldsType;
/* ***** OTFlags ******/
/*
   This type is used to describe bitwise flags in OT data structures
   and parameters.  Think of it as the OT analogue to the OptionBits
   type in "MacTypes.h".
*/

typedef UInt32                          OTFlags;
/*
   These flags are used when sending and receiving data.  The
   constants defined are masks.
*/
enum {
  T_MORE                        = 0x0001, /* More data to come in message     */
  T_EXPEDITED                   = 0x0002, /* Data is expedited, if possible */
  T_ACKNOWLEDGED                = 0x0004, /* Acknowledge transaction         */
  T_PARTIALDATA                 = 0x0008, /* Partial data - more coming     */
  T_NORECEIPT                   = 0x0010, /* No event on transaction done     */
  T_TIMEDOUT                    = 0x0020 /* Reply timed out              */
};

/* These flags are used in the TOptMgmt structure to request services.*/

enum {
  T_NEGOTIATE                   = 0x0004,
  T_CHECK                       = 0x0008,
  T_DEFAULT                     = 0x0010,
  T_CURRENT                     = 0x0080
};

/*
   These flags are used in the TOptMgmt and TOption structures to
   return results.
*/

enum {
  T_SUCCESS                     = 0x0020,
  T_FAILURE                     = 0x0040,
  T_PARTSUCCESS                 = 0x0100,
  T_READONLY                    = 0x0200,
  T_NOTSUPPORT                  = 0x0400
};

/*
   Mentat's code does an #ifdef on this symbol, so we have to #define
   it as well as declare it as an enum.  But only for Apple builds because
   we don't want internal weirdness to propagate to developers.
*/

#define T_CURRENT T_CURRENT
#define T_PARTSUCCESS T_PARTSUCCESS
#define T_NOTSUPPORT T_NOTSUPPORT
#define T_READONLY T_READONLY

/*
   ***** OTBand *****
   A band is a STREAMS concepts which defines the priority of data
   on a stream.  Although this type is defined as a 32 bit number
   for efficiency's sake, bands actually only range from 0 to 255. 
   Typically band 0 is used for normal data and band 1 for expedited data.
*/
typedef UInt32                          OTBand;
/* ***** Object References ******/
/*
   This deserves some explanation.  If you're compiling for
   C++, the C++ definitions of TEndpoint and TMapper at the
   end of this file are invoked, which lets the compiler
   know that they are both subclasses of TProvider.  This
   way the compiler will do the right subclass type checking,
   ie you will be able to pass an EndpointRef to a parameter
   of type ProviderRef, but not vice versa.
   On the other hand, if your compiling for straighth C,
   everything is defined as void.  This is somewhat dangerous,
   but it prevents you have to cast an EndpointRef to a
   ProviderRef every time you call a function that works
   on all forms of providers.
*/
#ifdef __cplusplus
  typedef class TProvider*  ProviderRef;
  typedef class TEndpoint*  EndpointRef;
  typedef class TMapper*    MapperRef;
#else
  typedef void*             ProviderRef;
  typedef void*             EndpointRef;
  typedef void*             MapperRef;
#endif
#define kOTInvalidRef   0L
#define kOTInvalidProviderRef   ((ProviderRef)0L)
#define kOTInvalidEndpointRef   ((EndpointRef)0L)
#define kOTInvalidMapperRef     ((MapperRef)0L)
/* ***** Event Codes ******/
/*
   OT event codes values for Open Transport.  These are the event codes that
   are sent to notification routine (notifiers).
*/

typedef UInt32                          OTEventCode;
/*
   Events are divided into numerous categories:
   
   1. (0x0000xxxx) The core XTI events have identifiers of the form
      T_XXXX.  These signal that an XTI event has occured on a stream.
   2. (0x1000xxxx) Private events are reserved for protocol specific
      events.  Each protocol stack defines them as appropriate for
      its own usage.
   3. (0x2000xxxxx) Completion events have identifiers of the form
      T_XXXXCOMPLETE.  These signal the completion of some asynchronous
      API routine, and are only delivered if the endpoint is in asynchronous
      mode.
   4. (0x2100xxxx) Stream events are generally encountered when programming
      the raw streams API and indicate some event on a raw stream, or
      some other event of interest in the STREAMS kernel.
   5. (0x2200xxxx) Signal events indicate that a signal has arrived on
      a raw stream.  See "Signal Values" for details.
   6. (0x2300xxxx) General provider events that might be generated by any
      provider.
   7. (0x2400xxxx) System events sent to all providers.
   8. (0x2500xxxx) System events sent to registered clients.
   9. (0x2600xxxx) System events used by configurators.
  10. (0x2700xxxx) Events sent to registered OT clients.
*/
/*
   All event codes not described here are reserved by Apple.  If you receive
   an event code you do not understand, ignore it!
*/

enum {
  T_LISTEN                      = 0x0001, /* An connection request is available     */
  T_CONNECT                     = 0x0002, /* Confirmation of a connect request  */
  T_DATA                        = 0x0004, /* Standard data is available        */
  T_EXDATA                      = 0x0008, /* Expedited data is available         */
  T_DISCONNECT                  = 0x0010, /* A disconnect is available       */
  T_ERROR                       = 0x0020, /* obsolete/unused in library        */
  T_UDERR                       = 0x0040, /* A Unit Data Error has occurred     */
  T_ORDREL                      = 0x0080, /* An orderly release is available       */
  T_GODATA                      = 0x0100, /* Flow control lifted on standard data   */
  T_GOEXDATA                    = 0x0200, /* Flow control lifted on expedited data*/
  T_REQUEST                     = 0x0400, /* An Incoming request is available     */
  T_REPLY                       = 0x0800, /* An Incoming reply is available     */
  T_PASSCON                     = 0x1000, /* State is now T_DATAXFER          */
  T_RESET                       = 0x2000, /* Protocol has been reset          */
  kPRIVATEEVENT                 = 0x10000000, /* Base of the private event range.*/
  kCOMPLETEEVENT                = 0x20000000, /* Base of the completion event range.*/
  T_BINDCOMPLETE                = 0x20000001, /* Bind call is complete          */
  T_UNBINDCOMPLETE              = 0x20000002, /* Unbind call is complete          */
  T_ACCEPTCOMPLETE              = 0x20000003, /* Accept call is complete          */
  T_REPLYCOMPLETE               = 0x20000004, /* SendReply call is complete        */
  T_DISCONNECTCOMPLETE          = 0x20000005, /* Disconnect call is complete         */
  T_OPTMGMTCOMPLETE             = 0x20000006, /* OptMgmt call is complete          */
  T_OPENCOMPLETE                = 0x20000007, /* An Open call is complete          */
  T_GETPROTADDRCOMPLETE         = 0x20000008, /* GetProtAddress call is complete       */
  T_RESOLVEADDRCOMPLETE         = 0x20000009, /* A ResolveAddress call is complet     */
  T_GETINFOCOMPLETE             = 0x2000000A, /* A GetInfo call is complete        */
  T_SYNCCOMPLETE                = 0x2000000B, /* A Sync call is complete          */
  T_MEMORYRELEASED              = 0x2000000C, /* No-copy memory was released         */
  T_REGNAMECOMPLETE             = 0x2000000D, /* A RegisterName call is complete       */
  T_DELNAMECOMPLETE             = 0x2000000E, /* A DeleteName call is complete   */
  T_LKUPNAMECOMPLETE            = 0x2000000F, /* A LookupName call is complete   */
  T_LKUPNAMERESULT              = 0x20000010, /* A LookupName is returning a name     */
  kOTSyncIdleEvent              = 0x20000011, /* Synchronous call Idle event         */
  kSTREAMEVENT                  = 0x21000000, /* Base of the raw stream event range.*/
  kOTReservedEvent1             = 0x21000001, /* reserved for internal use by OT       */
  kGetmsgEvent                  = 0x21000002, /* A GetMessage call is complete   */
  kStreamReadEvent              = 0x21000003, /* A Read call is complete          */
  kStreamWriteEvent             = 0x21000004, /* A Write call is complete          */
  kStreamIoctlEvent             = 0x21000005, /* An Ioctl call is complete       */
  kOTReservedEvent2             = 0x21000006, /* reserved for internal use by OT       */
  kStreamOpenEvent              = 0x21000007, /* An OpenStream call is complete     */
  kPollEvent                    = 0x21000008, /* A Poll call is complete          */
  kOTReservedEvent3             = 0x21000009, /* reserved for internal use by OT       */
  kOTReservedEvent4             = 0x2100000A, /* reserved for internal use by OT       */
  kOTReservedEvent5             = 0x2100000B, /* reserved for internal use by OT       */
  kOTReservedEvent6             = 0x2100000C, /* reserved for internal use by OT       */
  kOTReservedEvent7             = 0x2100000D, /* reserved for internal use by OT       */
  kOTReservedEvent8             = 0x2100000E, /* reserved for internal use by OT       */
  kSIGNALEVENT                  = 0x22000000, /* A signal has arrived on a raw stream, see "Signal Values" below.*/
  kPROTOCOLEVENT                = 0x23000000, /* Some event from the protocols   */
  kOTProviderIsDisconnected     = 0x23000001, /* Provider is temporarily off-line     */
  kOTProviderIsReconnected      = 0x23000002, /* Provider is now back on-line      */
  kOTProviderWillClose          = 0x24000001, /* Provider will close immediately       */
  kOTProviderIsClosed           = 0x24000002, /* Provider was closed              */
  kOTPortDisabled               = 0x25000001, /* Port is now disabled, result is 0, cookie is port ref */
  kOTPortEnabled                = 0x25000002, /* Port is now enabled, result is 0, cookie is port ref */
  kOTPortOffline                = 0x25000003, /* Port is now offline, result is 0, cookie is port ref */
  kOTPortOnline                 = 0x25000004, /* Port is now online, result is 0, cookie is port ref */
  kOTClosePortRequest           = 0x25000005, /* Request to close/yield, result is reason, cookie is OTPortCloseStruct* */
  kOTYieldPortRequest           = 0x25000005, /* Request to close/yield, result is reason, cookie is OTPortCloseStruct* */
  kOTNewPortRegistered          = 0x25000006, /* New port has been registered, cookie is port ref */
  kOTPortNetworkChange          = 0x25000007, /* Port may have moved to a new network, result is 0, cookie is port ref */
  kOTConfigurationChanged       = 0x26000001, /* Protocol configuration changed     */
  kOTSystemSleep                = 0x26000002,
  kOTSystemShutdown             = 0x26000003,
  kOTSystemAwaken               = 0x26000004,
  kOTSystemIdle                 = 0x26000005,
  kOTSystemSleepPrep            = 0x26000006,
  kOTSystemShutdownPrep         = 0x26000007,
  kOTSystemAwakenPrep           = 0x26000008,
  kOTStackIsLoading             = 0x27000001, /* Sent before Open Transport attempts to load the TCP/IP protocol stack.*/
  kOTStackWasLoaded             = 0x27000002, /* Sent after the TCP/IP stack has been successfully loaded.*/
  kOTStackIsUnloading           = 0x27000003 /* Sent before Open Transport unloads the TCP/IP stack.*/
};



/*
   The following event codes are used internally by Open Transport
   but not documented to developers.  I had to remove them from the
   above list because Interfacer won't let me put a hard conditional
   inside an enum declaration.
*/
enum {
  kOTDisablePortEvent           = 0x21000001,
  kStreamCloseEvent             = 0x21000006,
  kBackgroundStreamEvent        = 0x21000009,
  kIoctlRecvFdEvent             = 0x2100000A,
  kOTTryShutdownEvent           = 0x2100000B, /* probably not used by current OT (2.5)*/
  kOTScheduleTerminationEvent   = 0x2100000C,
  kOTEnablePortEvent            = 0x2100000D,
  kOTNewPortRegisteredEvent     = 0x2100000E,
  kOTPortOfflineEvent           = 0x2100000F,
  kOTPortOnlineEvent            = 0x21000010,
  kOTPortNetworkChangeEvent     = 0x21000011
};


/* ***** Event Classification Macros ***** */

#define IsOTPrivateEvent(x)         (((x) & 0x70000000L) == kPRIVATEEVENT)
#define IsOTCompleteEvent(x)     (((x) & 0x7f000000L) == kCOMPLETEEVENT)
#define IsOTProtocolEvent(x)        (((x) & 0x7f000000L) == kPROTOCOLEVENT)
#define IsOTStreamEvent(x)          (((x) & 0x7f000000L) == kSTREAMEVENT)
#define IsOTSignalEvent(x)            (((x) & 0x7f000000L) == kSIGNALEVENT)
#define GetOTEventCode(x)         (x)

/*
   ***** Signal Values *****
   Signals that are generated by a raw stream.  When writing a notifier
   for a raw stream, add these values to kSIGNALEVENT to determine what
   event you are receiving.
*/

enum {
  kSIGHUP                       = 1,
  kSIGURG                       = 16,
  kSIGPOLL                      = 30
};

#define SIGHUP  1
#define SIGURG  16

/*
   ***** Notifier Type Definition *****
   Open Transport notifiers must conform to the OTNotifyProcPtr prototype.
   Even though a OTNotifyUPP is a OTNotifyProcPtr on pre-Carbon system,
   use NewOTNotifyUPP() and friends to make your source code portable to OS X and Carbon.
*/

typedef CALLBACK_API( void , OTNotifyProcPtr )(void *contextPtr, OTEventCode code, OTResult result, void *cookie);
typedef STACK_UPP_TYPE(OTNotifyProcPtr)                         OTNotifyUPP;
/*
 *  NewOTNotifyUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OTNotifyUPP
NewOTNotifyUPP(OTNotifyProcPtr userRoutine)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;

/*
 *  DisposeOTNotifyUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeOTNotifyUPP(OTNotifyUPP userUPP)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;

/*
 *  InvokeOTNotifyUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeOTNotifyUPP(
  void *       contextPtr,
  OTEventCode  code,
  OTResult     result,
  void *       cookie,
  OTNotifyUPP  userUPP)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;

#if __MACH__
  #ifdef __cplusplus
    inline OTNotifyUPP                                          NewOTNotifyUPP(OTNotifyProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeOTNotifyUPP(OTNotifyUPP) { }
    inline void                                                 InvokeOTNotifyUPP(void * contextPtr, OTEventCode code, OTResult result, void * cookie, OTNotifyUPP userUPP) { (*userUPP)(contextPtr, code, result, cookie); }
  #else
    #define NewOTNotifyUPP(userRoutine)                         ((OTNotifyUPP)userRoutine)
    #define DisposeOTNotifyUPP(userUPP)
    #define InvokeOTNotifyUPP(contextPtr, code, result, cookie, userUPP) (*userUPP)(contextPtr, code, result, cookie)
  #endif
#endif

/* ***** Option Management Definitions ******/
/* The XTI Level number of a protocol.*/
enum {
  XTI_GENERIC                   = 0xFFFF /* level for XTI options */
};

typedef UInt32                          OTXTILevel;
/* The XTI name of a protocol option.*/
typedef UInt32                          OTXTIName;
/* XTI names for options used with XTI_GENERIC above*/
enum {
  XTI_DEBUG                     = 0x0001,
  XTI_LINGER                    = 0x0080,
  XTI_RCVBUF                    = 0x1002,
  XTI_RCVLOWAT                  = 0x1004,
  XTI_SNDBUF                    = 0x1001,
  XTI_SNDLOWAT                  = 0x1003,
  XTI_PROTOTYPE                 = 0x1005,
  OPT_CHECKSUM                  = 0x0600, /* Set checksumming = UInt32 - 0 or 1)*/
  OPT_RETRYCNT                  = 0x0601, /* Set a retry counter = UInt32 (0 = infinite)*/
  OPT_INTERVAL                  = 0x0602, /* Set a retry interval = UInt32 milliseconds*/
  OPT_ENABLEEOM                 = 0x0603, /* Enable the EOM indication = UInt8 (0 or 1)*/
  OPT_SELFSEND                  = 0x0604, /* Enable Self-sending on broadcasts = UInt32 (0 or 1)*/
  OPT_SERVERSTATUS              = 0x0605, /* Set Server Status (format is proto dependent)*/
  OPT_ALERTENABLE               = 0x0606, /* Enable/Disable protocol alerts*/
  OPT_KEEPALIVE                 = 0x0008 /* See t_keepalive structure*/
};

/* ***** Ioctl Definitions ******/

/*
   All OT ioctl numbers are formed using the MIOC_CMD macro,
   which divides the ioctl space by protocol space (the
   first parameter) and ioctl number within that protocol
   space (the second parameter).  This macro is only available
   to C users but it's relatively easy to synthesise its
   results in other languages.
*/
#define MIOC_CMD(t,v)   ((((t)&0xFF) << 8) | ((v)&0xFF))

/* The following is a registry of the ioctls protocol spaces.*/

enum {
  MIOC_STREAMIO                 = 'A',  /* Basic Stream ioctl() cmds - I_PUSH, I_LOOK, etc. */
  MIOC_TMOD                     = 'a',  /* ioctl's for tmod test module    */
  MIOC_STRLOG                   = 'b',  /* ioctl's for Mentat's log device      */
  MIOC_ND                       = 'c',  /* ioctl's for Mentat's nd device        */
  MIOC_ECHO                     = 'd',  /* ioctl's for Mentat's echo device   */
  MIOC_TLI                      = 'e',  /* ioctl's for Mentat's timod module  */
  MIOC_RESERVEDf                = 'f',  /* reserved, used by SVR4 FIOxxx   */
  MIOC_SAD                      = 'g',  /* ioctl's for Mentat's sad module      */
  MIOC_ARP                      = 'h',  /* ioctl's for Mentat's arp module      */
  MIOC_HAVOC                    = 'H',  /* Havoc module ioctls.           */
  MIOC_RESERVEDi                = 'i',  /* reserved, used by SVR4 SIOCxxx     */
  MIOC_SIOC                     = 'j',  /* sockio.h socket ioctl's           */
  MIOC_TCP                      = 'k',  /* tcp.h ioctl's                */
  MIOC_DLPI                     = 'l',  /* dlpi.h additions             */
  MIOC_SOCKETS                  = 'm',  /* Mentat sockmod ioctl's           */
  MIOC_IPX                      = 'o',  /* ioctls for IPX               */
  MIOC_OT                       = 'O',  /* ioctls for Open Transport       */
  MIOC_ATALK                    = 'T',  /* ioctl's for AppleTalk          */
  MIOC_SRL                      = 'U',  /* ioctl's for Serial           */
  MIOC_RESERVEDp                = 'p',  /* reserved, used by SVR4          */
  MIOC_RESERVEDr                = 'r',  /* reserved, used by SVR4          */
  MIOC_RESERVEDs                = 's',  /* reserved, used by SVR4          */
  MIOC_CFIG                     = 'z'   /* ioctl's for private configuration */
};

/* OT specific ioctls.*/

enum {
  I_OTGetMiscellaneousEvents    = ((MIOC_OT << 8) | 1), /* sign up for Misc Events              */
  I_OTSetFramingType            = ((MIOC_OT << 8) | 2), /* Set framing option for link          */
  kOTGetFramingValue            = (UInt32)0xFFFFFFFF, /* Use this value to read framing        */
  I_OTSetRawMode                = ((MIOC_OT << 8) | 3), /* Set raw mode for link            */
  kOTSetRecvMode                = 0x01,
  kOTSendErrorPacket            = 0x02,
  I_OTConnect                   = ((MIOC_OT << 8) | 4), /* Generic connect request for links   */
  I_OTDisconnect                = ((MIOC_OT << 8) | 5), /* Generic disconnect request for links     */
  I_OTScript                    = ((MIOC_OT << 8) | 6) /* Send a script to a module          */
};

/* Structure for the I_OTScript Ioctl.*/

struct OTScriptInfo {
  UInt32              fScriptType;
  void *              fTheScript;
  UInt32              fScriptLength;
};
typedef struct OTScriptInfo             OTScriptInfo;
/*
   ***** XTI States *****
   These are the potential values returned by OTGetEndpointState and OTSync
   which represent the XTI state of an endpoint.
*/
typedef UInt32 OTXTIStates;
enum {
  T_UNINIT                      = 0,    /* addition to standard xti.h */
  T_UNBND                       = 1,    /* unbound                 */
  T_IDLE                        = 2,    /* idle                */
  T_OUTCON                      = 3,    /* outgoing connection pending    */
  T_INCON                       = 4,    /* incoming connection pending    */
  T_DATAXFER                    = 5,    /* data transfer          */
  T_OUTREL                      = 6,    /* outgoing orderly release     */
  T_INREL                       = 7     /* incoming orderly release     */
};

/*
   ***** General XTI Definitions *****
   These definitions are typically used during option management.
*/

enum {
  T_YES                         = 1,
  T_NO                          = 0,
  T_UNUSED                      = (unsigned int)(-1),
  kT_NULL                       = 0,
  T_ABSREQ                      = 0x8000
};

enum {
  kT_UNSPEC                     = (UInt32)0xFFFFFFFD,
  T_ALLOPT                      = 0
};

/*
   T_NULL and T_UNSPEC have different values in BSD headers.  If you want the
   OT values, use kT_NULL or kT_UNSPEC.
*/
/*
   ***** OTConfiguration *****
   This is a "black box" structure used to define the configuration of a
   provider or endpoint.  This file defines a very limited set of operations
   on a configuration.  "OpenTransportClient.h" extends this with extra
   operations used by protocol stacks but not typically needed by clients.
*/


#ifdef __cplusplus
typedef class OTConfiguration*          OTConfigurationRef;
#else
typedef struct OTConfiguration*         OTConfigurationRef;
#endif
#define kOTNoMemoryConfigurationPtr     ((OTConfigurationRef)0L)
#define kOTInvalidConfigurationPtr      ((OTConfigurationRef)-1L)
/* ***** Option Management Structures ******/

/* This structure describes the contents of a single option in a buffer.*/

struct TOptionHeader {
  ByteCount           len;                    /* total length of option          */
                                              /* = sizeof(TOptionHeader) + length     */
                                              /*     of option value in bytes       */
  OTXTILevel          level;                  /* protocol affected            */
  OTXTIName           name;                   /* option name                   */
  UInt32              status;                 /* status value                */
};
typedef struct TOptionHeader            TOptionHeader;
/*
   This structure describes the contents of a single option in a buffer.
   It differs from TOptionHeader in that it includes the value field,
   which acts as an unbounded array representing the value of the option.
*/
struct TOption {
  ByteCount           len;                    /* total length of option          */
                                              /* = sizeof(TOption) + length */
                                              /*     of option value in bytes       */
  OTXTILevel          level;                  /* protocol affected            */
  OTXTIName           name;                   /* option name                   */
  UInt32              status;                 /* status value                */
  UInt32              value[1];               /* data goes here               */
};
typedef struct TOption                  TOption;
/* Some useful constants when manipulating option buffers.*/
enum {
  kOTOptionHeaderSize           = sizeof(TOptionHeader),
  kOTBooleanOptionDataSize      = sizeof(UInt32),
  kOTBooleanOptionSize          = kOTOptionHeaderSize + kOTBooleanOptionDataSize,
  kOTOneByteOptionSize          = kOTOptionHeaderSize + 1,
  kOTTwoByteOptionSize          = kOTOptionHeaderSize + 2,
  kOTFourByteOptionSize         = kOTOptionHeaderSize + sizeof(UInt32)
};


/*
    This macro will align return the value of "len", rounded up to the next
    4-byte boundary.
*/

#define T_ALIGN(len) (((UInt32)(len)+(sizeof(SInt32)-1)) & ~(sizeof(SInt32)-1))

/*
   This macro will return the next option in the buffer, given the previous option
    in the buffer, returning NULL if there are no more.
    You start off by setting prevOption = (TOption*)theBuffer
  (Use OTNextOption for a more thorough check - it ensures the end
   of the option is in the buffer as well.)
*/

#define OPT_NEXTHDR(theBuffer, theBufLen, prevOption) \
   (((char*)(prevOption) + T_ALIGN((prevOption)->len) < (char*)(theBuffer) + (theBufLen)) ?    \
          (TOption*)((char*)(prevOption)+T_ALIGN((prevOption)->len))  \
          : (TOption*)NULL)


/* t_kpalive is used with OPT_KEEPALIVE option.*/

struct t_kpalive {
  SInt32              kp_onoff;               /* option on/off   */
  SInt32              kp_timeout;             /* timeout in minutes */
};
typedef struct t_kpalive                t_kpalive;
/* t_linger is used with XTI_LINGER option.*/
struct t_linger {
  SInt32              l_onoff;                /* option on/off */
  SInt32              l_linger;               /* linger time */
};
typedef struct t_linger                 t_linger;
/*
   ***** TEndpointInfo *****
   This structure is returned from the GetEndpointInfo call and contains
   information about an endpoint.  But first, some special flags and types.
*/
/* Values returned in servtype field of TEndpointInfo.*/

typedef UInt32 OTServiceType;
enum {
  T_COTS                        = 1,    /* Connection-mode service                    */
  T_COTS_ORD                    = 2,    /* Connection service with orderly release          */
  T_CLTS                        = 3,    /* Connectionless-mode service                   */
  T_TRANS                       = 5,    /* Connection-mode transaction service              */
  T_TRANS_ORD                   = 6,    /* Connection transaction service with orderly release    */
  T_TRANS_CLTS                  = 7     /* Connectionless transaction service           */
};

/* Masks for the flags field of TEndpointInfo.*/

enum {
  T_SENDZERO                    = 0x0001, /* supports 0-length TSDU's          */
  T_XPG4_1                      = 0x0002, /* supports the GetProtAddress call     */
  T_CAN_SUPPORT_MDATA           = 0x10000000, /* support M_DATAs on packet protocols    */
  T_CAN_RESOLVE_ADDR            = 0x40000000, /* Supports ResolveAddress call      */
  T_CAN_SUPPLY_MIB              = 0x20000000 /* Supports SNMP MIB data          */
};

/*
   Special-case values for in the tsdu, etsdu, connect, and discon
   fields of TEndpointInfo.
*/

enum {
  T_INFINITE                    = -1,   /* supports infinit amounts of data     */
  T_INVALID                     = -2    /* Does not support data transmission */
};


typedef SInt32                          OTDataSize;
/* Now the TEndpointInfo structure proper.*/
struct TEndpointInfo {
  OTDataSize          addr;                   /* Maximum size of an address        */
  OTDataSize          options;                /* Maximum size of options          */
  OTDataSize          tsdu;                   /* Standard data transmit unit size     */
  OTDataSize          etsdu;                  /* Expedited data transmit unit size  */
  OTDataSize          connect;                /* Maximum data size on connect      */
  OTDataSize          discon;                 /* Maximum data size on disconnect       */
  OTServiceType       servtype;               /* service type                */
  UInt32              flags;                  /* Flags (see above for values)      */
};
typedef struct TEndpointInfo            TEndpointInfo;
/*
   "OpenTransport.h" no longer defines "struct t_info".  We recommend
   that you use TEndpointInfo instead.  If this is impossible, use
   the definition of "struct t_info" in "OpenTransportXTI.h".
*/
/* ***** OTPortRecord ******/

/* Unique identifier for a port.*/


typedef UInt32                          OTPortRef;
typedef OTPortRef *                     OTPortRefPtr;
enum {
  kOTInvalidPortRef             = 0
};

/* Valid values for the bus type element of an OTPortRef.*/

typedef UInt8 OTBusType;
enum {
  kOTUnknownBusPort             = 0,
  kOTMotherboardBus             = 1,
  kOTNuBus                      = 2,
  kOTPCIBus                     = 3,
  kOTGeoPort                    = 4,
  kOTPCCardBus                  = 5,
  kOTFireWireBus                = 6,
  kOTLastBusIndex               = 15
};

/*
   A couple of special values for the device type element of an
   OTPortRef.  See "OpenTransportDevices.h" for the standard values.
*/

typedef UInt16 OTDeviceType;
enum {
  kOTNoDeviceType               = 0,
  kOTADEVDevice                 = 1,    /* An Atalk ADEV   */
  kOTMDEVDevice                 = 2,    /* A TCP/IP MDEV   */
  kOTLocalTalkDevice            = 3,    /* LocalTalk       */
  kOTIRTalkDevice               = 4,    /* IRTalk          */
  kOTTokenRingDevice            = 5,    /* Token Ring        */
  kOTISDNDevice                 = 6,    /* ISDN             */
  kOTATMDevice                  = 7,    /* ATM              */
  kOTSMDSDevice                 = 8,    /* SMDS             */
  kOTSerialDevice               = 9,    /* Serial           */
  kOTEthernetDevice             = 10,   /* Ethernet          */
  kOTSLIPDevice                 = 11,   /* SLIP Pseudo-device */
  kOTPPPDevice                  = 12,   /* PPP Pseudo-device  */
  kOTModemDevice                = 13,   /* Modem Pseudo-Device    */
  kOTFastEthernetDevice         = 14,   /* 100 MB Ethernet       */
  kOTFDDIDevice                 = 15,   /* FDDI             */
  kOTIrDADevice                 = 16,   /* IrDA Infrared   */
  kOTATMSNAPDevice              = 17,   /* ATM SNAP emulation */
  kOTFibreChannelDevice         = 18,   /* Fibre Channel   */
  kOTFireWireDevice             = 19,   /* FireWire link Device   */
  kOTPseudoDevice               = 1023, /* used where no other defined device type will work*/
  kOTLastDeviceIndex            = 1022
};

/* Special case values for the slot number element of an OTPortRef.*/

enum {
  kOTLastSlotNumber             = 255,
  kOTLastOtherNumber            = 255
};

typedef UInt16                          OTSlotNumber;
/* Accessor functions for the various elements of the OTPortRef.*/
#if !__LP64__
/*
 *  OTCreatePortRef()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTPortRef 
OTCreatePortRef(
  OTBusType      busType,
  OTDeviceType   devType,
  OTSlotNumber   slot,
  UInt16         other)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTGetDeviceTypeFromPortRef()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTDeviceType 
OTGetDeviceTypeFromPortRef(OTPortRef ref)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTGetBusTypeFromPortRef()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern UInt16 
OTGetBusTypeFromPortRef(OTPortRef ref)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTGetSlotFromPortRef()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTSlotNumber 
OTGetSlotFromPortRef(
  OTPortRef   ref,
  UInt16 *    other)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTSetDeviceTypeInPortRef()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTPortRef 
OTSetDeviceTypeInPortRef(
  OTPortRef      ref,
  OTDeviceType   devType)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTSetBusTypeInPortRef()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTPortRef 
OTSetBusTypeInPortRef(
  OTPortRef   ref,
  OTBusType   busType)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;



/*
    Convenience macros for generating specific types of OTPortRefs.
*/

#define OTCreateNuBusPortRef(devType, slot, other)  \
  OTCreatePortRef(kOTNuBus, devType, slot, other)
    
#define OTCreatePCIPortRef(devType, slot, other)   \
  OTCreatePortRef(kOTPCIBus, devType, slot, other)
   
#define OTCreatePCCardPortRef(devType, slot, other)    \
  OTCreatePortRef(kOTPCCardBus, devType, slot, other)

/* Name length definitions for various fields in OTPortRecord.*/

#endif  /* !__LP64__ */

enum {
  kMaxModuleNameLength          = 31,   /* max length of a STREAMS module name*/
  kMaxModuleNameSize            = kMaxModuleNameLength + 1,
  kMaxProviderNameLength        = kMaxModuleNameLength + 4, /* providers allow 4 characters for minor number*/
  kMaxProviderNameSize          = kMaxProviderNameLength + 1,
  kMaxSlotIDLength              = 7,    /* PCI slot names tend to be short*/
  kMaxSlotIDSize                = kMaxSlotIDLength + 1,
  kMaxResourceInfoLength        = 31,   /* max length of a configuration helper name*/
  kMaxResourceInfoSize          = 32,
  kMaxPortNameLength            = kMaxModuleNameLength + 4, /* max size allowed to define a port*/
  kMaxPortNameSize              = kMaxPortNameLength + 1
};

/*
   Masks for the fPortFlags field of OTPortRecord
   If no bits are set, the port is currently inactive.
*/

enum {
  kOTPortIsActive               = 0x00000001,
  kOTPortIsDisabled             = 0x00000002,
  kOTPortIsUnavailable          = 0x00000004,
  kOTPortIsOffline              = 0x00000008
};

/* Masks for the fInfoFlags field of the OTPortRecord.*/

enum {
  kOTPortIsDLPI                 = 0x00000001,
  kOTPortIsTPI                  = 0x00000002,
  kOTPortCanYield               = 0x00000004, /* will not be set until the port is used for the first time*/
  kOTPortCanArbitrate           = 0x00000008, /* will not be set until the port is used for the first time*/
  kOTPortIsTransitory           = 0x00000010,
  kOTPortAutoConnects           = 0x00000020,
  kOTPortIsSystemRegistered     = 0x00004000,
  kOTPortIsPrivate              = 0x00008000,
  kOTPortIsAlias                = (UInt32)0x80000000
};

/*
   One OTPortRecord is created for each instance of a port.
   For Instance 'enet' identifies an ethernet port.
   A OTPortRecord for each ethernet card it finds, with an
   OTPortRef that will uniquely allow the driver to determine which
   port it is supposed to open on.
*/

struct OTPortRecord {
  OTPortRef           fRef;
  UInt32              fPortFlags;
  UInt32              fInfoFlags;
  UInt32              fCapabilities;
  ItemCount           fNumChildPorts;
  OTPortRef *         fChildPorts;
  char                fPortName[36];
  char                fModuleName[32];
  char                fSlotID[8];
  char                fResourceInfo[32];
  char                fReserved[164];
};
typedef struct OTPortRecord             OTPortRecord;
/*
   Routines for finding, registering and unregistering ports.
   IMPORTANT:
   These routines have two versions, one for the client and one
   for the kernel.  Make sure you use and link with the right ones.
*/
#if !OTKERNEL
#if !__LP64__
/*
 *  OTGetIndexedPort()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientUtilLib 1.0 and later
 */
extern Boolean 
OTGetIndexedPort(
  OTPortRecord *  portRecord,
  OTItemCount     index)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Index through the ports in the system*/
/*
 *  OTFindPort()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientUtilLib 1.0 and later
 */
extern Boolean 
OTFindPort(
  OTPortRecord *  portRecord,
  const char *    portName)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Find an OTPortRecord for a port using it's name*/
/*
 *  OTFindPortByRef()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientUtilLib 1.0 and later
 */
extern Boolean 
OTFindPortByRef(
  OTPortRecord *  portRecord,
  OTPortRef       ref)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Find an OTPortRecord for a port using it's OTPortRef*/
#endif  /* !__LP64__ */

/*
 *  OTRegisterPort()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientUtilLib 1.0 and later
 */


/*
   Register a port. The name the port was registered under is returned in
   the fPortName field.
*/
/*
 *  OTUnregisterPort()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientUtilLib 1.0 and later
 */


/*
   Unregister the port with the given name (If you re-register the
   port, it may get a different name - use OTChangePortState if
   that is not desireable).  Since a single OTPortRef can be registered
   with several names, the API needs to use the portName rather than
   the OTPortRef to disambiguate.
*/
/*
 *  OTChangePortState()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientUtilLib 1.0 and later
 */


/* Change the state of the port.*/
#endif  /* !OTKERNEL */

/* ***** Data Buffers ******/
/*
   TNetbuf is the basic structure used to pass data back and forth
   between the Open Transport protocols and their clients
*/

struct TNetbuf {
  ByteCount           maxlen;
  ByteCount           len;
  UInt8 *             buf;
};
typedef struct TNetbuf                  TNetbuf;
/*
   Some rarely used low-level routines in this file take a strbuf
   as a parameter.  This is the raw streams equivalent of a TNetbuf.
   The key difference is that the maxlen and len fields are signed,
   which allows you to specify extra operations by providing a
   negative value.
*/


struct strbuf {
  SInt32              maxlen;                 /* max buffer length */
  SInt32              len;                    /* length of data */
  char *              buf;                    /* pointer to buffer */
};
typedef struct strbuf                   strbuf;
/*
   OTData is used in a TNetbuf or netbuf to send
   non-contiguous data.  Set the 'len' field of the netbuf to the
   constant kNetbufDataIsOTData to signal that the 'buf' field of the
   netbuf actually points to one of these structures instead of a
   memory buffer.
*/
struct OTData {
  void *              fNext;
  void *              fData;
  ByteCount           fLen;
};
typedef struct OTData                   OTData;
enum {
  kNetbufDataIsOTData           = (UInt32)0xFFFFFFFE
};


/*
   OTBuffer is used for no-copy receives.  When receiving, you can
   set the receive length to kOTNetbufDataIsOTBufferStar and then
   pass the address of an OTBuffer* as the receive buffer.  OT will
   fill it out to point to a chain of OTBuffers.
   When you are done with it, you must call the OTReleaseBuffer function.
   For best performance, you need to call OTReleaseBuffer quickly.
   Only data netbufs may use this - no netbufs for addresses or options, or the like.
   Any OTBuffer returned to you by OT is read only!
   The astute will notice that this has a high correlation with the
   STREAMS msgb data type.  The fields are commented with their
   corresponding msgb field name.
*/

struct OTBuffer {
  void *              fLink;                  /* b_next*/
  void *              fLink2;                 /* b_prev*/
  struct OTBuffer *   fNext;                  /* b_cont*/
  UInt8 *             fData;                  /* b_rptr*/
  ByteCount           fLen;                   /* b_wptr*/
  void *              fSave;                  /* b_datap*/
  UInt8               fBand;                  /* b_band*/
  UInt8               fType;                  /* b_pad1*/
  UInt8               fPad1;
  UInt8               fFlags;                 /* b_flag*/
};
typedef struct OTBuffer                 OTBuffer;
enum {
  kOTNetbufDataIsOTBufferStar   = (UInt32)0xFFFFFFFD
};

/*
   OTBufferInfo is used with OTReadBuffer to keep track of where you
   are in the buffer, since the OTBuffer is "read-only".
*/
/* Use the OTInitBuffer macro to initialise this structure from an OTBuffer chain.*/
struct OTBufferInfo {
  OTBuffer *          fBuffer;
  ByteCount           fOffset;
  UInt8               fPad;
};
typedef struct OTBufferInfo             OTBufferInfo;

#define OTInitBufferInfo(infoPtr, theBuffer)   \
  (infoPtr)->fBuffer = theBuffer;             \
  (infoPtr)->fPad = (theBuffer)->fPad1;       \
  (infoPtr)->fOffset  = 0

/*
   If the endpoint supports "raw mode" (the T_CAN_SUPPORT_MDATA bit will
   be set in the TEndpointInfo::flags field), then you specify the
   raw mode packet by putting the kOTNetbufIsRawMode value in
   the udata.addr.len field when calling OTSndUData and also set the
   udata.opt.len, udata.opt.buf, and udata.addr.buf fields to 0.
*/

enum {
  kOTNetbufIsRawMode            = (UInt32)0xFFFFFFFF
};

/*
   ***** Standard XTI Parameter Types *****
   These structures are all used as parameters to the standard
   XTI routines.
*/

/*
   TBind holds binding information for calls to
   OTGetProtAddress, OTResolveAddress and OTBind.
*/

struct TBind {
  TNetbuf             addr;
  OTQLen              qlen;
};
typedef struct TBind                    TBind;
/*
   TDiscon is passed to RcvDisconnect to find out additional information
   about the disconnect.
*/
struct TDiscon {
  TNetbuf             udata;
  OTReason            reason;
  OTSequence          sequence;
};
typedef struct TDiscon                  TDiscon;
/*
   TCall holds information about a connection and is a parameter to
   OTConnect, OTRcvConnect, OTListen, OTAccept, and OTSndDisconnect.
*/
struct TCall {
  TNetbuf             addr;
  TNetbuf             opt;
  TNetbuf             udata;
  OTSequence          sequence;
};
typedef struct TCall                    TCall;
/* TUnitData describes a datagram in calls to OTSndUData and OTRcvUData.*/
struct TUnitData {
  TNetbuf             addr;
  TNetbuf             opt;
  TNetbuf             udata;
};
typedef struct TUnitData                TUnitData;
/*
   TUDErr is used to get information about a datagram error using
   OTRcvUDErr.
*/
struct TUDErr {
  TNetbuf             addr;
  TNetbuf             opt;
  SInt32              error;
};
typedef struct TUDErr                   TUDErr;
/* TOptMgmt is passed to the OTOptionManagement call to read or set protocol*/
struct TOptMgmt {
  TNetbuf             opt;
  OTFlags             flags;
};
typedef struct TOptMgmt                 TOptMgmt;
/*
   ***** Transactional XTI Parameter Types *****
   These structures are all used as parameters to the OT's
   XTI-like routines for transaction protocols.
*/
/*
   TRequest is passed to OTSndRequest and OTRcvRequest that contains the information
   about the request.
*/

struct TRequest {
  TNetbuf             data;
  TNetbuf             opt;
  OTSequence          sequence;
};
typedef struct TRequest                 TRequest;
/* TReply is passed to OTSndReply to send a reply to an incoming request.*/
struct TReply {
  TNetbuf             data;
  TNetbuf             opt;
  OTSequence          sequence;
};
typedef struct TReply                   TReply;
/*
   TUnitRequest is passed to OTSndURequest and OTRcvURequest that contains
   the information about the request.
*/
struct TUnitRequest {
  TNetbuf             addr;
  TNetbuf             opt;
  TNetbuf             udata;
  OTSequence          sequence;
};
typedef struct TUnitRequest             TUnitRequest;
/* TUnitReply is passed to OTSndUReply to send a reply to an incoming request.*/
struct TUnitReply {
  TNetbuf             opt;
  TNetbuf             udata;
  OTSequence          sequence;
};
typedef struct TUnitReply               TUnitReply;
/*
   ***** Mapper Parameter Types *****
   These structures are all used as parameters to the OT's
   mapper routines.
*/
/* TRegisterRequest holds the name to register in a call to OTRegisterName.*/

struct TRegisterRequest {
  TNetbuf             name;
  TNetbuf             addr;
  OTFlags             flags;
};
typedef struct TRegisterRequest         TRegisterRequest;
/*
   TRegisterReply returns information about the registered name in a call
   to OTRegisterName.
*/
struct TRegisterReply {
  TNetbuf             addr;
  OTNameID            nameid;
};
typedef struct TRegisterReply           TRegisterReply;
/* TLookupRequest holds the name to look up in a call to OTLookupName.*/
struct TLookupRequest {
  TNetbuf             name;
  TNetbuf             addr;
  UInt32              maxcnt;
  OTTimeout           timeout;
  OTFlags             flags;
};
typedef struct TLookupRequest           TLookupRequest;
/*
   TLookupReply returns information about the found names after a call
   to OTLookupName.
*/
struct TLookupReply {
  TNetbuf             names;
  UInt32              rspcount;
};
typedef struct TLookupReply             TLookupReply;
/*
   TLookupBuffer describes the contents of the names buffer pointed
   to by the TLookupReply.
*/
struct TLookupBuffer {
  UInt16              fAddressLength;
  UInt16              fNameLength;
  UInt8               fAddressBuffer[1];
};
typedef struct TLookupBuffer            TLookupBuffer;

/*
    OTNextLookupBuffer allows you to step through a packed array
   of TLookupBuffers.
*/

#define OTNextLookupBuffer(buf)          \
  ((TLookupBuffer*)                   \
      ((char*)buf + ((OTOffsetOf(TLookupBuffer, fAddressBuffer) + buf->fAddressLength + buf->fNameLength + 3) & ~3)))

/* ***** Initializing and Shutting Down Open Transport ******/

#if !OTKERNEL
typedef struct OpaqueOTClientContextPtr*  OTClientContextPtr;
/*
   For Carbon the InitOpenTransport interface has changed so it takes a flags parameter 
   and returns a client context pointer.
   The flag passed to indicates whether OT should be initialized for application use or for some other target
   (for example, plugins that run in an application context but not the application itself.)
   Applications that are not interested in the value of the client context pointer may pass NULL
   as outClientContext -- they will pass NULL to other routines that take a OTClientContextPtr.
*/
typedef UInt32 OTInitializationFlags;
enum {
  kInitOTForApplicationMask     = 1,
  kInitOTForExtensionMask       = 2
};

#if !__LP64__
/*
 *  InitOpenTransportInContext()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
InitOpenTransportInContext(
  OTInitializationFlags   flags,
  OTClientContextPtr *    outClientContext)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;



/*
   Under Carbon, CloseOpenTransport takes a client context pointer.  Applications may pass NULL
   after calling InitOpenTransport(kInitOTForApplicationMask, ...).  Non-applications must always pass a
   valid client context.
*/
/*
 *  CloseOpenTransportInContext()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
CloseOpenTransportInContext(OTClientContextPtr clientContext) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;



#endif  /* !__LP64__ */

/*
 *  InitOpenTransport()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


/*
 *  InitOpenTransportUtilities()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


/*
 *  CloseOpenTransport()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


/*
 *  OTRegisterAsClient()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


/*
   This registers yourself as a client for any miscellaneous Open Transport
   notifications that come along. CloseOpenTransport will automatically do
   an OTUnregisterAsClient, if you have not already done so.
*/
/*
 *  OTUnregisterAsClient()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


#if !__LP64__
/*
 *  OTRegisterAsClientInContext()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
OTRegisterAsClientInContext(
  OTClientName         name,
  OTNotifyUPP          proc,
  OTClientContextPtr   clientContext)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTUnregisterAsClientInContext()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
OTUnregisterAsClientInContext(OTClientContextPtr clientContext) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


#endif  /* !__LP64__ */

#if OTCARBONAPPLICATION
/* The following macro may be used by applications only.*/
#define InitOpenTransport() InitOpenTransportInContext(kInitOTForApplicationMask, NULL)
#define CloseOpenTransport() CloseOpenTransportInContext(NULL)
#define OTRegisterAsClient(name, proc) OTRegisterAsClientInContext(name, proc, NULL)
#define OTUnregisterAsClient() OTUnregisterAsClientInContext(NULL)
#endif  /* OTCARBONAPPLICATION */

#endif  /* !OTKERNEL */

/* ***** Tasking Model ******/
/*
   OTEnterInterrupt/OTLeaveInterrupt are normally used within the kernel to
   tell Open Transport we're at hardware interrupt time.  Clients can also
   them to do the same.
*/

/*
 *  OTEnterInterrupt()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */


/*
 *  OTLeaveInterrupt()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */


/*
 *  OTIsAtInterruptLevel()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */


/*
 *  OTCanLoadLibraries()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */


/*
   All OT task callbacks use the same prototype, shown below.
   This is only a UPP for CFM-68K clients.
*/

typedef CALLBACK_API( void , OTProcessProcPtr )(void * arg);
typedef STACK_UPP_TYPE(OTProcessProcPtr)                        OTProcessUPP;
/*
 *  NewOTProcessUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OTProcessUPP
NewOTProcessUPP(OTProcessProcPtr userRoutine)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;

/*
 *  DisposeOTProcessUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeOTProcessUPP(OTProcessUPP userUPP)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;

/*
 *  InvokeOTProcessUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeOTProcessUPP(
  void *        arg,
  OTProcessUPP  userUPP)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;

#if __MACH__
  #ifdef __cplusplus
    inline OTProcessUPP                                         NewOTProcessUPP(OTProcessProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeOTProcessUPP(OTProcessUPP) { }
    inline void                                                 InvokeOTProcessUPP(void * arg, OTProcessUPP userUPP) { (*userUPP)(arg); }
  #else
    #define NewOTProcessUPP(userRoutine)                        ((OTProcessUPP)userRoutine)
    #define DisposeOTProcessUPP(userUPP)
    #define InvokeOTProcessUPP(arg, userUPP)                    (*userUPP)(arg)
  #endif
#endif

#if !OTKERNEL
/*
   Under Carbon, OTCreateDeferredTask takes a client context pointer.  Applications may pass NULL
   after calling InitOpenTransport(kInitOTForApplicationMask, ...).  Non-applications must always pass a
   valid client context.
*/
#if !__LP64__
/*
 *  OTCreateDeferredTaskInContext()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern long 
OTCreateDeferredTaskInContext(
  OTProcessUPP         upp,
  void *               arg,
  OTClientContextPtr   clientContext)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


#endif  /* !__LP64__ */

#endif  /* !OTKERNEL */

/*
   OT deferred tasks are often more convenience that standard Mac OS
   although they have no significant advantages beyond convenience.
*/


typedef long                            OTDeferredTaskRef;
/*
 *  OTCreateDeferredTask()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


#if !__LP64__
/*
 *  OTScheduleDeferredTask()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern Boolean 
OTScheduleDeferredTask(OTDeferredTaskRef dtCookie)            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


#endif  /* !__LP64__ */

/*
 *  OTScheduleInterruptTask()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */


#if !__LP64__
/*
 *  OTDestroyDeferredTask()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OSStatus 
OTDestroyDeferredTask(OTDeferredTaskRef dtCookie)             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


#endif  /* !__LP64__ */

#if OTCARBONAPPLICATION
/* The following macro may be used by applications only.*/
#define OTCreateDeferredTask(upp, arg) OTCreateDeferredTaskInContext(upp, arg, NULL)
#endif  /* OTCARBONAPPLICATION */

#if !OTKERNEL
/*
   OT system tasks allow you to schedule a procedure to be called
   at system task time.  Potentially useful, but it relies on someone
   calling SystemTask (or WaitNextEvent, which calls SystemTask).
   Not available to kernel code because relying on system task time
   to make progress is likely to result in deadlocks.
*/
typedef long                            OTSystemTaskRef;
/*
 *  OTCreateSystemTask()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


/*
 *  OTDestroySystemTask()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */


/*
 *  OTScheduleSystemTask()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */


/*
 *  OTCancelSystemTask()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */


#if !__LP64__
/*
 *  OTCanMakeSyncCall()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern Boolean 
OTCanMakeSyncCall(void)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


#endif  /* !__LP64__ */

#endif  /* !OTKERNEL */

/* ***** Interface to Providers ******/
#if !OTKERNEL
/*
 *  OTAsyncOpenProvider()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


/*
 *  OTOpenProvider()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


#if !__LP64__
/*
 *  OTCloseProvider()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
OTCloseProvider(ProviderRef ref)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


#endif  /* !__LP64__ */

/*
 *  OTTransferProviderOwnership()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


/*
 *  OTWhoAmI()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


/*
 *  OTGetProviderPortRef()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


#if !__LP64__
/*
 *  OTIoctl()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern SInt32 
OTIoctl(
  ProviderRef   ref,
  UInt32        cmd,
  void *        data)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


#endif  /* !__LP64__ */

/*
 *  OTGetMessage()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


/*
 *  OTGetPriorityMessage()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


/*
 *  OTPutMessage()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


/*
 *  OTPutPriorityMessage()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


#if !__LP64__
/*
 *  OTSetAsynchronous()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTSetAsynchronous(ProviderRef ref)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTSetSynchronous()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTSetSynchronous(ProviderRef ref)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTIsSynchronous()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern Boolean 
OTIsSynchronous(ProviderRef ref)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTSetBlocking()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTSetBlocking(ProviderRef ref)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTSetNonBlocking()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTSetNonBlocking(ProviderRef ref)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTIsBlocking()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern Boolean 
OTIsBlocking(ProviderRef ref)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTInstallNotifier()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTInstallNotifier(
  ProviderRef   ref,
  OTNotifyUPP   proc,
  void *        contextPtr)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTUseSyncIdleEvents()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTUseSyncIdleEvents(
  ProviderRef   ref,
  Boolean       useEvents)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTRemoveNotifier()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern void 
OTRemoveNotifier(ProviderRef ref)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTLeaveNotifier()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern void 
OTLeaveNotifier(ProviderRef ref)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTEnterNotifier()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern Boolean 
OTEnterNotifier(ProviderRef ref)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTAckSends()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTAckSends(ProviderRef ref)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTDontAckSends()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTDontAckSends(ProviderRef ref)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTIsAckingSends()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern Boolean 
OTIsAckingSends(ProviderRef ref)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTCancelSynchronousCalls()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTCancelSynchronousCalls(
  ProviderRef   ref,
  OSStatus      err)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;



#define OTIsNonBlocking(ref)   (!OTIsBlocking(ref))
#define OTIsAsynchronous(ref)  (!OTIsSynchronous(ref))

#endif  /* !__LP64__ */

#endif  /* !OTKERNEL */

/* ***** Interface to Endpoints ******/
#if !OTKERNEL
/* Open/Close*/
/*
   Under Carbon, the OpenEndpoint routines take a client context pointer.  Applications may pass NULL after
   calling InitOpenTransport(kInitOTForApplicationMask, ...).  Non-applications must always pass a
   valid client context.
*/
#if !__LP64__
/*
 *  OTOpenEndpointInContext()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern EndpointRef 
OTOpenEndpointInContext(
  OTConfigurationRef   config,
  OTOpenFlags          oflag,
  TEndpointInfo *      info,                /* can be NULL */
  OSStatus *           err,
  OTClientContextPtr   clientContext)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTAsyncOpenEndpointInContext()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
OTAsyncOpenEndpointInContext(
  OTConfigurationRef   config,
  OTOpenFlags          oflag,
  TEndpointInfo *      info,                /* can be NULL */
  OTNotifyUPP          upp,
  void *               contextPtr,
  OTClientContextPtr   clientContext)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;




#endif  /* !__LP64__ */

/*
 *  OTOpenEndpoint()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


/*
 *  OTAsyncOpenEndpoint()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


#if OTCARBONAPPLICATION
/* The following macros may be used by applications only.*/
#define OTOpenEndpoint(config, oflag, info, err)  OTOpenEndpointInContext(config, oflag, info, err, NULL)
#define OTAsyncOpenEndpoint(config, oflag, info, proc, contextPtr)  OTAsyncOpenEndpointInContext(config, oflag, info, proc, contextPtr, NULL)
#endif  /* OTCARBONAPPLICATION */

/* Misc Information*/

#if !__LP64__
/*
 *  OTGetEndpointInfo()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTGetEndpointInfo(
  EndpointRef      ref,
  TEndpointInfo *  info)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTGetEndpointState()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OTResult 
OTGetEndpointState(EndpointRef ref)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTLook()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OTResult 
OTLook(EndpointRef ref)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


#endif  /* !__LP64__ */

/*
 *  OTSync()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


#if !__LP64__
/*
 *  OTCountDataBytes()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OTResult 
OTCountDataBytes(
  EndpointRef    ref,
  OTByteCount *  countPtr)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTGetProtAddress()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTGetProtAddress(
  EndpointRef   ref,
  TBind *       boundAddr,       /* can be NULL */
  TBind *       peerAddr)        /* can be NULL */            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTResolveAddress()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTResolveAddress(
  EndpointRef   ref,
  TBind *       reqAddr,
  TBind *       retAddr,
  OTTimeout     timeOut)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Allocating structures*/

/*
   Note:
   In general, Apple recommends that you avoid the OTAlloc call because
   using it extensively causes your program to allocate and deallocate
   many memory blocks, with each extra memory allocation costing time.
*/

/*
   Under Carbon, OTAlloc takes a client context pointer.  Applications may pass NULL after
   calling InitOpenTransport(kInitOTForApplicationMask, ...).  Non-applications must always pass a
   valid client context.
*/
/*
 *  OTAllocInContext()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern void * 
OTAllocInContext(
  EndpointRef          ref,
  OTStructType         structType,
  UInt32               fields,
  OSStatus *           err,
  OTClientContextPtr   clientContext)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


#endif  /* !__LP64__ */

/*
 *  OTAlloc()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


#if OTCARBONAPPLICATION
/* The following macro may be used by applications only.*/
#define OTAlloc(ref, structType, fields, err) OTAllocInContext(ref, structType, fields, err, NULL)
#endif  /* OTCARBONAPPLICATION */

#if !__LP64__
/*
 *  OTFree()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OTResult 
OTFree(
  void *         ptr,
  OTStructType   structType)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Option management*/

/* It looks simple enough...*/

/*
 *  OTOptionManagement()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTOptionManagement(
  EndpointRef   ref,
  TOptMgmt *    req,
  TOptMgmt *    ret)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* ... but then the hidden complexity emerges.*/

#endif  /* !__LP64__ */

/*
 *  OTCreateOptions()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientUtilLib 1.0 and later
 */


/*
 *  OTCreateOptionString()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientUtilLib 1.0 and later
 */


#if !__LP64__
/*
 *  OTNextOption()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTNextOption(
  UInt8 *     buffer,
  UInt32      buflen,
  TOption **  prevOptPtr)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTFindOption()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern TOption * 
OTFindOption(
  UInt8 *      buffer,
  UInt32       buflen,
  OTXTILevel   level,
  OTXTIName    name)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Bind/Unbind*/

/*
 *  OTBind()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTBind(
  EndpointRef   ref,
  TBind *       reqAddr,       /* can be NULL */
  TBind *       retAddr)       /* can be NULL */              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTUnbind()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTUnbind(EndpointRef ref)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Connection creation/tear-down*/

/*
 *  OTConnect()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTConnect(
  EndpointRef   ref,
  TCall *       sndCall,
  TCall *       rcvCall)       /* can be NULL */              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTRcvConnect()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTRcvConnect(
  EndpointRef   ref,
  TCall *       call)       /* can be NULL */                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTListen()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTListen(
  EndpointRef   ref,
  TCall *       call)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTAccept()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTAccept(
  EndpointRef   listener,
  EndpointRef   worker,
  TCall *       call)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTSndDisconnect()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTSndDisconnect(
  EndpointRef   ref,
  TCall *       call)       /* can be NULL */                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTSndOrderlyDisconnect()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTSndOrderlyDisconnect(EndpointRef ref)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTRcvDisconnect()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTRcvDisconnect(
  EndpointRef   ref,
  TDiscon *     discon)       /* can be NULL */               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTRcvOrderlyDisconnect()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTRcvOrderlyDisconnect(EndpointRef ref)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Connection-oriented send/receive*/

/*
 *  OTRcv()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OTResult 
OTRcv(
  EndpointRef   ref,
  void *        buf,
  OTByteCount   nbytes,
  OTFlags *     flags)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTSnd()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OTResult 
OTSnd(
  EndpointRef   ref,
  void *        buf,
  OTByteCount   nbytes,
  OTFlags       flags)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Connectionless send/receive*/

/*
 *  OTSndUData()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTSndUData(
  EndpointRef   ref,
  TUnitData *   udata)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTRcvUData()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTRcvUData(
  EndpointRef   ref,
  TUnitData *   udata,
  OTFlags *     flags)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTRcvUDErr()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTRcvUDErr(
  EndpointRef   ref,
  TUDErr *      uderr)       /* can be NULL */                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Connection-oriented transactions*/

#endif  /* !__LP64__ */

/*
 *  OTSndRequest()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


/*
 *  OTRcvReply()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


/*
 *  OTSndReply()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


/*
 *  OTRcvRequest()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


/*
 *  OTCancelRequest()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


/*
 *  OTCancelReply()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


/* Connectionless transactions*/

/*
 *  OTSndURequest()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


/*
 *  OTRcvUReply()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


/*
 *  OTSndUReply()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


/*
 *  OTRcvURequest()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


/*
 *  OTCancelURequest()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


/*
 *  OTCancelUReply()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */


/* Interface to Mappers*/


/*
   Under Carbon, the OpenMapper routines take a client context pointer.  Applications may pass NULL after
   calling InitOpenTransport(kInitOTForApplicationMask, ...).  Non-applications must always pass a
   valid client context.
*/

#if !__LP64__
/*
 *  OTAsyncOpenMapperInContext()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
OTAsyncOpenMapperInContext(
  OTConfigurationRef   config,
  OTOpenFlags          oflag,
  OTNotifyUPP          upp,
  void *               contextPtr,
  OTClientContextPtr   clientContext)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTOpenMapperInContext()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern MapperRef 
OTOpenMapperInContext(
  OTConfigurationRef   config,
  OTOpenFlags          oflag,
  OSStatus *           err,
  OTClientContextPtr   clientContext)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;



#endif  /* !__LP64__ */

/*
 *  OTAsyncOpenMapper()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


/*
 *  OTOpenMapper()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


#if OTCARBONAPPLICATION
/* The following macros may be used by applications only.*/
#define OTAsyncOpenMapper(config, oflag, proc, contextPtr) OTAsyncOpenMapperInContext(config, oflag, proc, contextPtr, NULL)
#define OTOpenMapper(config, oflag, err) OTOpenMapperInContext(config, oflag, err, NULL)
#endif  /* OTCARBONAPPLICATION */

#if !__LP64__
/*
 *  OTRegisterName()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTRegisterName(
  MapperRef           ref,
  TRegisterRequest *  req,
  TRegisterReply *    reply)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTDeleteName()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTDeleteName(
  MapperRef   ref,
  TNetbuf *   name)                                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTDeleteNameByID()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTDeleteNameByID(
  MapperRef   ref,
  OTNameID    nameID)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTLookupName()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern OSStatus 
OTLookupName(
  MapperRef         ref,
  TLookupRequest *  req,
  TLookupReply *    reply)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Basic configuration manipulation*/

/*
 *  OTCreateConfiguration()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientUtilLib 1.0 and later
 */
extern OTConfigurationRef 
OTCreateConfiguration(const char * path)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTCloneConfiguration()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientUtilLib 1.0 and later
 */
extern OTConfigurationRef 
OTCloneConfiguration(OTConfigurationRef cfig)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTDestroyConfiguration()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientUtilLib 1.0 and later
 */
extern void 
OTDestroyConfiguration(OTConfigurationRef cfig)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
   This file defines a very limited set of operations
   on a configuration.  "OpenTransportClient.h" extends this with extra
   operations used by protocol stacks but not typically needed by clients.
*/

/* Interrupt-safe memory allocators*/

/*
   Under Carbon, OTAllocMem takes a client context pointer.  Applications may pass NULL after
   calling InitOpenTransport(kInitOTForApplicationMask, ...).  Non-applications must always pass a
   valid client context.
*/

/*
 *  OTAllocMemInContext()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern void * 
OTAllocMemInContext(
  OTByteCount          size,
  OTClientContextPtr   clientContext)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


#endif  /* !__LP64__ */

/*
 *  OTAllocMem()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */


#if !__LP64__
/*
 *  OTFreeMem()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientUtilLib 1.0 and later
 */
extern void 
OTFreeMem(void * mem)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


#endif  /* !__LP64__ */

#if OTCARBONAPPLICATION
/* The following macros may be used by applications only.*/
#define OTAllocMem(s) OTAllocMemInContext(s, NULL)
#endif  /* OTCARBONAPPLICATION */

/* Miscellaneous and Generic Routines*/

/*
   Neither of these routines should be necessary to the correct
   operation of an OT program.  If you're calling them, think again.
*/

#if !__LP64__
/*
 *  OTDelay()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern void 
OTDelay(UInt32 seconds)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTIdle()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTClientLib 1.0 and later
 */
extern void 
OTIdle(void)                                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


#endif  /* !__LP64__ */

#endif  /* !OTKERNEL */

/*
   ***** Open Transport Utility Routines *****
   All of these routines are available to both client and kernel.
*/
/* Memory and String Routines*/

/*
   These are preferable, especially in the kernel case, to the standard
   C equivalents because they don't require you to link with StdCLib.
*/

#if !__LP64__
/*
 *  OTMemcpy()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern void 
OTMemcpy(
  void *        dest,
  const void *  src,
  OTByteCount   nBytes)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTMemcmp()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern Boolean 
OTMemcmp(
  const void *  mem1,
  const void *  mem2,
  OTByteCount   nBytes)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTMemmove()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern void 
OTMemmove(
  void *        dest,
  const void *  src,
  OTByteCount   nBytes)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTMemzero()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern void 
OTMemzero(
  void *        dest,
  OTByteCount   nBytes)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTMemset()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern void 
OTMemset(
  void *         dest,
  OTUInt8Param   toSet,
  OTByteCount    nBytes)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTStrLength()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTByteCount 
OTStrLength(const char * str)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTStrCopy()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern void 
OTStrCopy(
  char *        dest,
  const char *  src)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTStrCat()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern void 
OTStrCat(
  char *        dest,
  const char *  src)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTStrEqual()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern Boolean 
OTStrEqual(
  const char *  src1,
  const char *  src2)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Timer Utilities*/

/*
   OTGetTimeStamp returns time in "tick" numbers, stored in 64 bits.
   This timestamp can be used as a base number for calculating elapsed 
   time.
   OTSubtractTimeStamps returns a pointer to the "result" parameter.
    
   OTGetClockTimeInSecs returns time since Open Transport was initialized
   in seconds.
*/

#endif  /* !__LP64__ */

typedef UnsignedWide                    OTTimeStamp;
#if !__LP64__
/*
 *  OTGetTimeStamp()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern void 
OTGetTimeStamp(OTTimeStamp * currentTime)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTSubtractTimeStamps()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTTimeStamp * 
OTSubtractTimeStamps(
  OTTimeStamp *  result,
  OTTimeStamp *  startTime,
  OTTimeStamp *  endEnd)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTTimeStampInMilliseconds()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern UInt32 
OTTimeStampInMilliseconds(OTTimeStamp * delta)                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTTimeStampInMicroseconds()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern UInt32 
OTTimeStampInMicroseconds(OTTimeStamp * delta)                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTElapsedMilliseconds()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern UInt32 
OTElapsedMilliseconds(OTTimeStamp * startTime)                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTElapsedMicroseconds()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern UInt32 
OTElapsedMicroseconds(OTTimeStamp * startTime)                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  OTGetClockTimeInSecs()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern UInt32 
OTGetClockTimeInSecs(void)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* ***** OT Link Element ******/

/*
   When using OT linked lists, all pointers to other elements are
   represented by the OTLink structure.  When operating on link
   lists, you always pass in the address of the OTLink on which
   list elements are chained.
*/

#endif  /* !__LP64__ */

struct OTLink {
  struct OTLink *     fNext;

#ifdef __cplusplus

   // C++ inline methods on this structure.
   
   void    Init()                      { fNext = NULL; }
#endif


};
typedef struct OTLink                   OTLink;

/*
    You can use this macro to map from an OTLink field to the
  structure in which it's embedded.
*/
#define OTGetLinkObject(link, struc, field)    \
  ((struc*)((char*)(link) - OTOffsetOf(struc, field)))

/* OTLIFO*/

/*
   These are functions to implement a LIFO list that is interrupt-safe.
   The only function which is not is OTReverseList.  Normally, you create
   a LIFO list, populate it at interrupt time, and then use OTLIFOStealList
   to atomically remove the list, and OTReverseList to flip the list so that
   it is a FIFO list, which tends to be more useful.
*/

struct OTLIFO {
  OTLink *            fHead;

#ifdef __cplusplus

   // C++ inline methods on this structure.

  void    Init();
    void    Enqueue(OTLink* link);
 OTLink* Dequeue();
 OTLink* StealList();
   Boolean IsEmpty();
#endif


};
typedef struct OTLIFO                   OTLIFO;
/*
   This function atomically enqueues the link onto the
   front of the list.
*/
#if !__LP64__
/*
 *  OTLIFOEnqueue()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern void 
OTLIFOEnqueue(
  OTLIFO *  list,
  OTLink *  link)                                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
   This function atomically dequeues the first element
   on the list.
*/
/*
 *  OTLIFODequeue()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTLink * 
OTLIFODequeue(OTLIFO * list)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
   This function atomically empties the list and returns a
   pointer to the first element on the list.
*/
/*
 *  OTLIFOStealList()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTLink * 
OTLIFOStealList(OTLIFO * list)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
   This function reverses a list that was stolen by
   OTLIFOStealList.  It is NOT atomic.  It returns the
   new starting list.
*/
/*
 *  OTReverseList()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTLink * 
OTReverseList(OTLink * list)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;



#ifdef __cplusplus

   // C++ inline methods on this structure.

  inline  void    OTLIFO::Init()                      { fHead = NULL; }
  inline  void    OTLIFO::Enqueue(OTLink* link)       { OTLIFOEnqueue(this, link); }
 inline  OTLink* OTLIFO::Dequeue()                   { return OTLIFODequeue(this); }
    inline  OTLink* OTLIFO::StealList()                 { return OTLIFOStealList(this); }
  inline  Boolean OTLIFO::IsEmpty()                   { return fHead == NULL; }
#endif

/* OTList*/

/*
   An OTList is a non-interrupt-safe list, but has more features than the
   OTLIFO list. It is a standard singly-linked list.
*/

/*
   The following is the prototype for a list element comparison function,
   which returns true if the element described by linkToCheck matches
   the client criteria (typically held in ref).
   This is only a UPP for CFM-68K clients.
*/

#endif  /* !__LP64__ */

typedef CALLBACK_API_C( Boolean , OTListSearchProcPtr )(const void *ref, OTLink *linkToCheck);
typedef STACK_UPP_TYPE(OTListSearchProcPtr)                     OTListSearchUPP;
/*
 *  NewOTListSearchUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OTListSearchUPP
NewOTListSearchUPP(OTListSearchProcPtr userRoutine)           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;

/*
 *  DisposeOTListSearchUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeOTListSearchUPP(OTListSearchUPP userUPP)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;

/*
 *  InvokeOTListSearchUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern Boolean
InvokeOTListSearchUPP(
  const void *     ref,
  OTLink *         linkToCheck,
  OTListSearchUPP  userUPP)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;

#if __MACH__
  #ifdef __cplusplus
    inline OTListSearchUPP                                      NewOTListSearchUPP(OTListSearchProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeOTListSearchUPP(OTListSearchUPP) { }
    inline Boolean                                              InvokeOTListSearchUPP(const void * ref, OTLink * linkToCheck, OTListSearchUPP userUPP) { return (*userUPP)(ref, linkToCheck); }
  #else
    #define NewOTListSearchUPP(userRoutine)                     ((OTListSearchUPP)userRoutine)
    #define DisposeOTListSearchUPP(userUPP)
    #define InvokeOTListSearchUPP(ref, linkToCheck, userUPP)    (*userUPP)(ref, linkToCheck)
  #endif
#endif

struct OTList {
  OTLink *            fHead;

#ifdef __cplusplus
    // C++ inline methods on this structure.

  void        Init();
    Boolean     IsEmpty();
 void        AddFirst(OTLink* link);
    void        AddLast(OTLink* link);
 OTLink*     GetFirst();
    OTLink*     GetLast();
 OTLink*     RemoveFirst();
 OTLink*     RemoveLast();
  Boolean     IsInList(OTLink* link);
    OTLink*     FindLink(OTListSearchUPP proc, const void* ref);
   Boolean     RemoveLink(OTLink* link);
  OTLink*     FindAndRemoveLink(OTListSearchUPP proc, const void* ref);
  OTLink*     GetIndexedLink(OTItemCount index);
#endif


};
typedef struct OTList                   OTList;
/* Add the link to the list at the front*/
#if !__LP64__
/*
 *  OTAddFirst()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern void 
OTAddFirst(
  OTList *  list,
  OTLink *  link)                                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Add the link to the list at the end*/
/*
 *  OTAddLast()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern void 
OTAddLast(
  OTList *  list,
  OTLink *  link)                                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Remove the first link from the list*/
/*
 *  OTRemoveFirst()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTLink * 
OTRemoveFirst(OTList * list)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Remove the last link from the list*/
/*
 *  OTRemoveLast()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTLink * 
OTRemoveLast(OTList * list)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Return the first link from the list*/
/*
 *  OTGetFirst()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTLink * 
OTGetFirst(OTList * list)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Return the last link from the list*/
/*
 *  OTGetLast()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTLink * 
OTGetLast(OTList * list)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Return true if the link is present in the list*/
/*
 *  OTIsInList()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern Boolean 
OTIsInList(
  OTList *  list,
  OTLink *  link)                                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
   Find a link in the list which matches the search criteria
   established by the search proc and the refPtr.  This is done
   by calling the search proc, passing it the refPtr and each
   link in the list, until the search proc returns true.
   NULL is returned if the search proc never returned true.
*/
/*
 *  OTFindLink()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTLink * 
OTFindLink(
  OTList *          list,
  OTListSearchUPP   proc,
  const void *      ref)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Remove the specified link from the list, returning true if it was found*/
/*
 *  OTRemoveLink()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern Boolean 
OTRemoveLink(
  OTList *  list,
  OTLink *  link)                                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Similar to OTFindLink, but it also removes it from the list.*/
/*
 *  OTFindAndRemoveLink()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTLink * 
OTFindAndRemoveLink(
  OTList *          list,
  OTListSearchUPP   proc,
  const void *      ref)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Return the "index"th link in the list*/
/*
 *  OTGetIndexedLink()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern OTLink * 
OTGetIndexedLink(
  OTList *      list,
  OTItemCount   index)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* OTEnqueue/OTDequeue*/

/*
   These routines are atomic, mighty weird, and generally not
   worth the complexity.  If you need atomic list operations,
   use OTLIFO instead.
*/

/*
   This function puts "object" on the listHead, and places the
   previous value at listHead into the pointer at "object" plus
   linkOffset.
*/
/*
 *  OTEnqueue()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern void 
OTEnqueue(
  void **       listHead,
  void *        object,
  OTByteCount   linkOffset)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
   This function returns the head object of the list, and places
   the pointer at "object" + linkOffset into the listHead
*/
/*
 *  OTDequeue()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern void * 
OTDequeue(
  void **       listHead,
  OTByteCount   linkOffset)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;



#ifdef __cplusplus
    // C++ inline methods on this structure.

  inline  void        OTList::Init()                  { fHead = NULL; }   
   inline  Boolean     OTList::IsEmpty()               { return fHead == NULL; }
  inline  void        OTList::AddFirst(OTLink* link)  { OTAddFirst(this, link); }
    inline  void        OTList::AddLast(OTLink* link)   { OTAddLast(this, link); }
 inline  OTLink*     OTList::GetFirst()              { return OTGetFirst(this); }
   inline  OTLink*     OTList::GetLast()               { return OTGetLast(this); }
    inline  OTLink*     OTList::RemoveFirst()           { return OTRemoveFirst(this); }
    inline  OTLink*     OTList::RemoveLast()            { return OTRemoveLast(this); }
 inline  Boolean     OTList::IsInList(OTLink* link)  { return OTIsInList(this, link); }
 inline  OTLink*     OTList::FindLink(OTListSearchUPP proc, const void* ref)
                                                        { return OTFindLink(this, proc, ref); }
    inline  Boolean     OTList::RemoveLink(OTLink* link) { return OTRemoveLink(this, link); }
  inline  OTLink*     OTList::FindAndRemoveLink(OTListSearchUPP proc, const void* ref)
                                                       { return OTFindAndRemoveLink(this, proc, ref); }
   inline  OTLink*     OTList::GetIndexedLink(OTItemCount index)
                                                      { return OTGetIndexedLink(this, index); }
#endif

/* Atomic Operations*/

/*
   Note:
   The Bit operations return the previous value of the bit (0 or non-zero).
   The memory pointed to must be a single byte and only bits 0 through 7 are
   valid.  Bit 0 corresponds to a mask of 0x01, and Bit 7 to a mask of 0x80.
*/

/*
   WARNING!
   void* and UInt32 locations MUST be on 4-byte boundaries.
   UInt16 locations must not cross a 4-byte boundary.
*/

/*
 *  OTAtomicSetBit()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern Boolean 
OTAtomicSetBit(
  UInt8 *       bytePtr,
  OTByteCount   bitNumber)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
   bset.b d0,(a0)
   sne d0
   moveq #1,d1
   and.l d1,d0
*/
/*
 *  OTAtomicClearBit()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern Boolean 
OTAtomicClearBit(
  UInt8 *       bytePtr,
  OTByteCount   bitNumber)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
   bclr.b d0,(a0)
   sne d0
   moveq #1,d1
   and.l d1,d0
*/
/*
 *  OTAtomicTestBit()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern Boolean 
OTAtomicTestBit(
  UInt8 *       bytePtr,
  OTByteCount   bitNumber)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
   btst.b d0,(a0)
   sne d0 *|
   moveq #1,d1
   and.l d1,d0 *|
*/
/*
 *  OTCompareAndSwapPtr()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern Boolean 
OTCompareAndSwapPtr(
  void *   oldValue,
  void *   newValue,
  void **  dest)                                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
   cas.l    d0,d1,(a0)  *|
   seq      d0          *|
   moveq #1,d1; and.l d1,d0 *|
*/
/*
 *  OTCompareAndSwap32()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern Boolean 
OTCompareAndSwap32(
  UInt32    oldValue,
  UInt32    newValue,
  UInt32 *  dest)                                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
   cas.l    d0,d1,(a0)  *|
   seq      d0          *|
   moveq #1,d1; and.l d1,d0 *|
*/
/*
 *  OTCompareAndSwap16()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern Boolean 
OTCompareAndSwap16(
  UInt32    oldValue,
  UInt32    newValue,
  UInt16 *  dest)                                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
   cas.w    d0,d1,(a0)  *|
   seq      d0          *|
   moveq #1,d1; and.l d1,d0 *|
*/
/*
 *  OTCompareAndSwap8()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern Boolean 
OTCompareAndSwap8(
  UInt32   oldValue,
  UInt32   newValue,
  UInt8 *  dest)                                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
   cas.b    d0,d1,(a0)  *|
   seq      d0          *|
   moveq #1,d1; and.l d1,d0 *|
*/
/*
 *  OTAtomicAdd32()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern SInt32 
OTAtomicAdd32(
  SInt32    toAdd,
  SInt32 *  dest)                                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
   move.l   d0,a1       *|
   move.l   (a0),d1     *|
   move.l   d1,d0       *|
   add.l    a1,d0       *|
   cas.l    d1,d0,(a0)  *|
   bne.s    @1          *|
*/
/*
 *  OTAtomicAdd16()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern SInt16 
OTAtomicAdd16(
  SInt32    toAdd,
  SInt16 *  dest)                                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Not used frequently enough to justify inlining.*/
/*
 *  OTAtomicAdd8()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in OTUtilityLib 1.0 and later
 */
extern SInt8 
OTAtomicAdd8(
  SInt32   toAdd,
  SInt8 *  dest)                                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* Not used frequently enough to justify inlining.*/
/* OTLock is just a convenience type with some convenient macros.*/

#endif  /* !__LP64__ */


typedef UInt8                           OTLock;

#define OTClearLock(lockPtr)   *(lockPtr) = 0
#define OTAcquireLock(lockPtr)   (OTAtomicSetBit(lockPtr, 0) == 0)

/*******************************************************************************
**
** FROM HERE ON DOWN ARE THE C++ Interfaces to Open Transport
**
********************************************************************************/

#ifdef __cplusplus
}                   // Terminate C definitions

/*  -------------------------------------------------------------------------
    CLASS TProvider
    
    This class provides the client interface to a Stream.  Typically, clients
    talk to an object (or glue code in front of the object) that is a subclass
    of TProvider.
    ------------------------------------------------------------------------- */

#if !OTKERNEL

class TProvider
{
    private:
        void*       operator new(size_t);
        void        operator delete(void*)  {}
        
    //
    // This is the public interface to a TProvider.  All other public
    // methods normally come from the subclass.
    //
    public: 
        OSStatus    Close()             { return OTCloseProvider(this); }

        OSStatus    SetNonBlocking()    { return OTSetNonBlocking(this); }
        OSStatus    SetBlocking()       { return OTSetBlocking(this); }
        Boolean     IsBlocking()        { return OTIsBlocking(this); }
        Boolean     IsNonBlocking()     { return !OTIsBlocking(this); }
        OSStatus    SetSynchronous()    { return OTSetSynchronous(this); }
        OSStatus    SetAsynchronous()   { return OTSetAsynchronous(this); }
        Boolean     IsSynchronous()     { return OTIsSynchronous(this); }
        Boolean     IsAsynchronous()    { return !OTIsSynchronous(this); }
        
        OSStatus    AckSends()          { return OTAckSends(this); }
        OSStatus    DontAckSends()      { return OTDontAckSends(this); }
        Boolean     IsAckingSends()     { return OTIsAckingSends(this); }

        void        CancelSynchronousCalls(OSStatus err)
                    { (void)OTCancelSynchronousCalls(this, err); }

        OSStatus    InstallNotifier(OTNotifyUPP proc, void* ptr)
                    { return OTInstallNotifier(this, proc, ptr); }

        OSStatus    UseSyncIdleEvents()
                    { return OTUseSyncIdleEvents(this, true); }

        OSStatus    DontUseSyncIdleEvents()
                    { return OTUseSyncIdleEvents(this, false); }
                    
        void        RemoveNotifier()
                    { OTRemoveNotifier(this); }
        
        Boolean     EnterNotifier()
                    { return OTEnterNotifier(this); }
                    
        void        LeaveNotifier()
                    { OTLeaveNotifier(this); }
                    
#if CALL_NOT_IN_CARBON
        OTPortRef   GetOTPortRef()
                    { return OTGetProviderPortRef(this); }
                    
        ProviderRef TransferOwnership(OTClient prevOwner, OSStatus* errPtr)
                    { return OTTransferProviderOwnership(this, prevOwner, errPtr); }
#endif
                    
        SInt32      Ioctl(UInt32 cmd, void* data)
                    { return OTIoctl(this, cmd, data); }
        SInt32      Ioctl(UInt32 cmd, long data)
                    { return OTIoctl(this, cmd, (void*)data); }
                    
#if CALL_NOT_IN_CARBON
        OTResult    GetMessage(strbuf* ctlbuf, strbuf* databuf, OTFlags* flagPtr)
                    { return OTGetMessage(this, ctlbuf, databuf, flagPtr); }
        OTResult    GetPriorityMessage(strbuf* ctlbuf, strbuf* databuf,
                                       OTBand* bandPtr, OTFlags* flagPtr)
                    { return OTGetPriorityMessage(this, ctlbuf, databuf, bandPtr, flagPtr); }
        OSStatus    PutMessage(const strbuf* ctlbuf, const strbuf* databuf,
                               OTFlags flags)
                    { return OTPutMessage(this, ctlbuf, databuf, flags); }
        OSStatus    PutPriorityMessage(const strbuf* ctlbuf, const strbuf* databuf,
                                       OTBand band, OTFlags flags)
                    { return OTPutPriorityMessage(this, ctlbuf, databuf, band, flags); }
#endif
                    

    public:
        void*   fData;
};
        
/* Prevent collision with some BSD definitons of macro Free() */
#ifdef Free
#undef Free
#endif
        
/*  -------------------------------------------------------------------------
    Class TEndpoint
    
    This class is the interface to all TPI modules, which constitute the
    vast majority of protocols, with the exception of link-layer protocols.
    ------------------------------------------------------------------------- */

class TEndpoint : public TProvider
{
    public:
    //
    // Miscellaneous informative functions
    //
        OSStatus    GetEndpointInfo(TEndpointInfo* info)
                    { return OTGetEndpointInfo(this, info); }
                    
        OSStatus    GetProtAddress(TBind* boundAddr, TBind* peerAddr)
                    { return OTGetProtAddress(this, boundAddr, peerAddr); }
                    
        OSStatus    ResolveAddress(TBind* reqAddr, TBind* retAddr, OTTimeout timeout)
                    { return OTResolveAddress(this, reqAddr, retAddr, timeout); }
                    
        OTResult    GetEndpointState()      
                    { return OTGetEndpointState(this); }
                    
        OTResult    Look()
                    { return OTLook(this); }
                    
#if CALL_NOT_IN_CARBON
        OTResult    Sync()
                    { return OTSync(this); }
#endif
    //
    // Allocating structures
    //
        void*       AllocInContext(OTStructType structType, UInt32 fields, OSStatus* err = NULL, OTClientContextPtr clientContext = NULL)
                    { return OTAllocInContext(this, structType, fields, err, clientContext); }
                    
        void*       Alloc(OTStructType structType, UInt32 fields, OSStatus* err = NULL)
                    {
                #if TARGET_API_MAC_CARBON
                         return OTAllocInContext(this, structType, fields, err, NULL);
                #else
                         return OTAlloc(this, structType, fields, err);
                #endif
                    };
                    
        OTResult    Free(void* ptr, OTStructType structType)
                    { return OTFree(ptr, structType); }
    //
    // Option Management
    //
        OSStatus    OptionManagement(TOptMgmt* req, TOptMgmt* ret)
                    { return OTOptionManagement(this, req, ret); }
    //
    // Bind/Unbind
    //
        OSStatus    Bind(TBind* reqAddr, TBind* retAddr)
                    { return OTBind(this, reqAddr, retAddr); }
                    
        OSStatus    Unbind()
                    { return OTUnbind(this); }
    //
    // Connection creation and tear-down
    //
        OSStatus    Connect(TCall* sndCall, TCall* rcvCall)
                    { return OTConnect(this, sndCall, rcvCall); }
                    
        OSStatus    RcvConnect(TCall* call)
                    { return OTRcvConnect(this, call); }
                    
        OSStatus    Listen(TCall* call)
                    { return OTListen(this, call); }
                    
        OSStatus    Accept(EndpointRef resRef, TCall* call)
                    { return OTAccept(this, resRef, call); }
                    
        OSStatus    SndDisconnect(TCall* call)
                    { return OTSndDisconnect(this, call); }
                    
        OSStatus    SndOrderlyDisconnect()
                    { return OTSndOrderlyDisconnect(this); }
                    
        OSStatus    RcvDisconnect(TDiscon* discon)
                    { return OTRcvDisconnect(this, discon); }
                    
        OSStatus    RcvOrderlyDisconnect()
                    { return OTRcvOrderlyDisconnect(this); }
    //
    // Connection-oriented data transfer
    //
        OTResult    Snd(void* buf, OTByteCount nbytes, OTFlags flags)
                    { return OTSnd(this, buf, nbytes, flags); }
                    
        OTResult    Rcv(void* buf, OTByteCount nbytes, OTFlags* flagP)
                    { return OTRcv(this, buf, nbytes, flagP); }
    //
    // Non-connection-oriented data transfer
    //
        OSStatus    SndUData(TUnitData* udata)
                    { return OTSndUData(this, udata); }
                    
        OSStatus    RcvUData(TUnitData* udata, OTFlags* flagP)
                    { return OTRcvUData(this, udata, flagP); }
                    
        OSStatus    RcvUDErr(TUDErr* uderr)
                    { return OTRcvUDErr(this, uderr); }

#if CALL_NOT_IN_CARBON
    //
    // Connection-oriented transactions
    //
        OSStatus    SndRequest(TRequest* req, OTFlags reqFlags)
                    { return OTSndRequest(this, req, reqFlags); }

        OSStatus    RcvReply(TReply* reply, OTFlags* replyFlags)
                    { return OTRcvReply(this, reply, replyFlags); }
                    
        OSStatus    SndReply(TReply* reply, OTFlags flags)
                    { return OTSndReply(this, reply, flags); }
                    
        OSStatus    RcvRequest(TRequest* req, OTFlags* flags)
                    { return OTRcvRequest(this, req, flags); }
                    
        OSStatus    CancelRequest(OTSequence seq)
                    { return OTCancelRequest(this, seq); }
                    
        OSStatus    CancelReply(OTSequence seq)
                    { return OTCancelReply(this, seq); }
    //
    // Non-connection-oriented transactions
    //
        OSStatus    SndURequest(TUnitRequest* req, OTFlags reqFlags)
                    { return OTSndURequest(this, req, reqFlags); }

        OSStatus    RcvUReply(TUnitReply* reply, OTFlags* replyFlags)
                    { return OTRcvUReply(this, reply, replyFlags); }
                    
        OSStatus    SndUReply(TUnitReply* reply, OTFlags flags)
                    { return OTSndUReply(this, reply, flags); }
                    
        OSStatus    RcvURequest(TUnitRequest* req, OTFlags* flags)
                    { return OTRcvURequest(this, req, flags); }
                    
        OSStatus    CancelURequest(OTSequence seq)
                    { return OTCancelURequest(this, seq); }
                    
        OSStatus    CancelUReply(OTSequence seq)
                    { return OTCancelUReply(this, seq); }
#endif
    //
    // Miscellaneous functions
    //
        OTResult    CountDataBytes(OTByteCount* countPtr)
                    { return OTCountDataBytes(this, countPtr); }
};

/*  -------------------------------------------------------------------------
    CLASS TMapper
    
    This class is the interface to naming protocols.
    ------------------------------------------------------------------------- */

class TMapper : public TProvider
{
    public:
        OSStatus    RegisterName(TRegisterRequest* req, TRegisterReply* reply)
                    { return OTRegisterName(this, req, reply); }
                
        OSStatus    DeleteName(TNetbuf* name)
                    { return OTDeleteName(this, name); }
                
        OSStatus    DeleteName(OTNameID theID)  
                    { return OTDeleteNameByID(this, theID); }
                
        OSStatus    LookupName(TLookupRequest* req, TLookupReply* reply)
                    { return OTLookupName(this, req, reply); }
};

#endif  /* !OTKERNEL    */

extern "C" {        // resume C definitions
#endif  /* __cplusplus */


#if defined(__MWERKS__) && TARGET_CPU_68K
 #pragma pop
#endif


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __OPENTRANSPORT__ */

