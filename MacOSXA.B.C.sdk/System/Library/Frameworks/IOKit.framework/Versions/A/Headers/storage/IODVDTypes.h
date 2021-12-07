/*
 * Copyright (c) 2000-2001 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef	_IODVDTYPES_H
#define	_IODVDTYPES_H

#include <IOKit/IOTypes.h>

/*
 *	The following CPRM Information is taken from Mt. Fuji
 *	Specifications ( INf-8090i Rev 4.0 ) document pp. 425-426
 */

typedef UInt8	DVDCPRMRegionCode;
enum
{
	kDVDCPRMRegion1		= 0xFE,
	kDVDCPRMRegion2		= 0xFD,
	kDVDCPRMRegion3		= 0xFB,
	kDVDCPRMRegion4		= 0xF7,
	kDVDCPRMRegion5		= 0xEF,
	kDVDCPRMRegion6		= 0xDF
};

typedef UInt8	DVDRegionalPlaybackControlScheme;
enum
{
	kDVDRegionalPlaybackControlSchemePhase1		= 0x00,
	kDVDRegionalPlaybackControlSchemePhase2		= 0x01
};

typedef UInt8	DVDBookType;
enum
{
	kDVDBookTypeROM		= 0x0,
	kDVDBookTypeRAM		= 0x1,
	kDVDBookTypeR		= 0x2,
	kDVDBookTypePlusRW	= 0x9
};

enum DVDKeyClass
{
	kDVDKeyClassCSS_CPPM_CPRM	= 0x00,
	kDVDKeyClassRSSA			= 0x01
};
typedef enum DVDKeyClass DVDKeyClass;

enum DVDKeyFormat
{
	kDVDKeyFormatAGID_CSS			= 0x00,
	kDVDKeyFormatChallengeKey		= 0x01,
	kDVDKeyFormatKey1				= 0x02,
	kDVDKeyFormatKey2				= 0x03,
	kDVDKeyFormatTitleKey			= 0x04,
	kDVDKeyFormatASF				= 0x05,
	kDVDKeyFormatSetRegion			= 0x06,
	kDVDKeyFormatRegionState		= 0x08,
	kDVDKeyFormatAGID_CSS2			= 0x10,
	kDVDKeyFormatAGID_CPRM			= 0x11,
	kDVDKeyFormatAGID_Invalidate	= 0x3F
};	
typedef enum DVDKeyFormat DVDKeyFormat;

typedef UInt8 	DVDStructureFormat;
enum
{
	kDVDStructureFormatPhysicalFormatInfo			= 0x00,
	kDVDStructureFormatCopyrightInfo				= 0x01,
	kDVDStructureFormatDiscKeyInfo					= 0x02,
	// skip BCA
	kDVDStructureFormatManufacturingInfo			= 0x04
};

// Read DVD Structures Format 0x00
struct DVDPhysicalFormatInfo
{
	UInt8		dataLength[2];
	UInt8		reserved[2];
#if defined(__LITTLE_ENDIAN__)
	// Byte 0
	UInt8		partVersion:4;
	UInt8		bookType:4;
	
	// Byte 1
	UInt8		minimumRate:4;
	UInt8		discSize:4;
	
	// Byte 2
	UInt8		layerType:4;
	UInt8		trackPath:1;
	UInt8		numberOfLayers:2;
	UInt8		reserved2:1;
	
	// Byte 3
	UInt8		trackDensity:4;
	UInt8		linearDensity:4;
#else /* !defined(__LITTLE_ENDIAN__) */
	// Byte 0
	UInt8		bookType:4;
	UInt8		partVersion:4;
	
	// Byte 1
	UInt8		discSize:4;
	UInt8		minimumRate:4;
	
	// Byte 2
	UInt8		reserved2:1;
	UInt8		numberOfLayers:2;
	UInt8		trackPath:1;
	UInt8		layerType:4;
	
	// Byte 3
	UInt8		linearDensity:4;
	UInt8		trackDensity:4;
#endif /* !defined(__LITTLE_ENDIAN__) */
	
	// Bytes 4-15
	UInt8		zero1;		// always 0x00
	UInt8		startingPhysicalSectorNumberOfDataArea[3];
	UInt8		zero2;		// always 0x00
	UInt8		endPhysicalSectorNumberOfDataArea[3];
	UInt8		zero3;		// always 0x00
	UInt8		endSectorNumberInLayerZero[3];
	
	// Byte 16
#if defined(__LITTLE_ENDIAN__)
	UInt8		reserved1:7;
	UInt8		bcaFlag:1;
#else /* !defined(__LITTLE_ENDIAN__) */
	UInt8		bcaFlag:1;
	UInt8		reserved1:7;
#endif /* !defined(__LITTLE_ENDIAN__) */
	
	// Bytes 17-2047
	UInt8		mediaSpecific[2031];
};
typedef struct DVDPhysicalFormatInfo DVDPhysicalFormatInfo;

// Read DVD Structures Format 0x01
struct DVDCopyrightInfo
{
	UInt8				dataLength[2];
	UInt8				reserved[2];
	UInt8				copyrightProtectionSystemType;
	DVDCPRMRegionCode	regionMask;
	UInt8				reserved2[2];
};
typedef struct DVDCopyrightInfo DVDCopyrightInfo;

// Read DVD Structures Format 0x02
struct DVDDiscKeyInfo
{
	UInt8		dataLength[2];
	UInt8		reserved[2];
	UInt8		discKeyStructures[2048];
};
typedef struct DVDDiscKeyInfo DVDDiscKeyInfo;

// Read DVD Structures Format 0x04
struct DVDManufacturingInfo
{
	UInt8		dataLength[2];
	UInt8		reserved[2];
	UInt8		discManufacturingInfo[2048];
};
typedef struct DVDManufacturingInfo DVDManufacturingInfo;

// ReportKey Format 0x00
struct DVDAuthenticationGrantIDInfo
{
	UInt8	dataLength[2];
	UInt8	reserved[2];
	UInt8	reserved2[3];
#if defined(__LITTLE_ENDIAN__)
	UInt8	reservedBits:6;
	UInt8	grantID:2;
#else /* !defined(__LITTLE_ENDIAN__) */
	UInt8	grantID:2;
	UInt8	reservedBits:6;
#endif /* !defined(__LITTLE_ENDIAN__) */
};
typedef struct DVDAuthenticationGrantIDInfo DVDAuthenticationGrantIDInfo;

// ReportKey and SendKey Format 0x01
struct DVDChallengeKeyInfo
{
	UInt8	dataLength[2];
	UInt8	reserved[2];
	UInt8	challengeKeyValue[10];
	UInt8	reserved2[2];
};
typedef struct DVDChallengeKeyInfo DVDChallengeKeyInfo;

// ReportKey Format 0x02
struct DVDKey1Info
{
	UInt8	dataLength[2];
	UInt8	reserved[2];
	UInt8	key1Value[5];
	UInt8	reserved2[3];
};
typedef struct DVDKey1Info DVDKey1Info;

// SendKey Format 0x03
struct DVDKey2Info
{
	UInt8	dataLength[2];
	UInt8	reserved[2];
	UInt8	key2Value[5];
	UInt8	reserved2[3];
};
typedef struct DVDKey2Info DVDKey2Info;

// ReportKey Format 0x04
struct DVDTitleKeyInfo
{
	UInt8	dataLength[2];
	UInt8	reserved[2];
#if defined(__LITTLE_ENDIAN__)
	UInt8	CP_MOD:4;
	UInt8	CGMS:2;
	UInt8	CP_SEC:1;
	UInt8	CPM:1;
#else 	/* !defined(__LITTLE_ENDIAN__) */
	UInt8	CPM:1;
	UInt8	CP_SEC:1;
	UInt8	CGMS:2;
	UInt8	CP_MOD:4;
#endif	/* !defined(__LITTLE_ENDIAN__) */
	UInt8	titleKeyValue[5];
	UInt8	reserved2[2];
};
typedef struct DVDTitleKeyInfo DVDTitleKeyInfo;

// ReportKey Format 0x05
struct DVDAuthenticationSuccessFlagInfo
{
	UInt8	dataLength[2];
	UInt8	reserved[2];
	UInt8	reserved2[3];
#if defined(__LITTLE_ENDIAN__)
	UInt8	successFlag:1;
	UInt8	reservedBits:7;
#else /* !defined(__LITTLE_ENDIAN__) */
	UInt8	reservedBits:7;
	UInt8	successFlag:1;
#endif /* !defined(__LITTLE_ENDIAN__) */
};
typedef struct DVDAuthenticationSuccessFlagInfo DVDAuthenticationSuccessFlagInfo;

// ReportKey Format 0x08
struct DVDRegionPlaybackControlInfo
{
	UInt8									dataLength[2];
	UInt8									reserved[2];
	#if defined(__LITTLE_ENDIAN__)
	UInt8									numberUserResets:3;
	UInt8									numberVendorResets:3;
	UInt8									typeCode:2;
	#else /* !defined(__LITTLE_ENDIAN__) */
	UInt8									typeCode:2;
	UInt8									numberVendorResets:3;
	UInt8									numberUserResets:3;
	#endif /* !defined(__LITTLE_ENDIAN__) */
	DVDCPRMRegionCode						driveRegion;
	DVDRegionalPlaybackControlScheme		rpcScheme;
	UInt8									reserved2;
};
typedef struct DVDRegionPlaybackControlInfo DVDRegionPlaybackControlInfo;

enum DVDMediaType
{
	kDVDMediaTypeUnknown   = 0x0200,
	kDVDMediaTypeROM       = 0x0202, /* DVD-ROM */
	kDVDMediaTypeRAM       = 0x0203, /* DVD-RAM */
	kDVDMediaTypeR         = 0x0204, /* DVD-R   */
	kDVDMediaTypeRW        = 0x0205, /* DVD-RW  */
	kDVDMediaTypePlusRW    = 0x0206, /* DVD+RW  */
	
	kDVDMediaTypeMin       = 0x0200,
	kDVDMediaTypeMax       = 0x02FF
};
typedef enum DVDMediaType DVDMediaType;

#define kDVDSpeedMin 0x0546
#define kDVDSpeedMax 0xFFFF

#endif /* _IODVDTYPES_H */