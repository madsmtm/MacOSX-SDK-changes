/*
     File:       PMPrintAETypes.r
 
     Contains:   Mac OS X Printing Manager AE definitions.
 
     Version:    Technology: Mac OS X
                 Release:    1.0
 
     Copyright (c) 2003,2008 by Apple Inc. All Rights Reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
*/

#ifndef __PMPrintAETypes_R__
#define __PMPrintAETypes_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define	kPMPrintSettingsAEType			'pset'
#define kPMShowPrintDialogAEType		'pdlg'
#define kPMPrinterAEType                'trpr'

#define kPMCopiesAEProp					"copies"
#define kPMCopiesAEKey					'lwcp'
#define kPMCopieAEType					typeSInt32

#define kPMCollateAEProp				"collating"
#define kPMCollateAEKey					'lwcl'
#define kPMCollateAEType				typeBoolean

#define kPMFirstPageAEProp				"starting page"
#define kPMFirstPageAEKey				'lwfp'
#define kPMFirstPageAEType				typeSInt32

#define kPMLastPageAEProp				"ending page"
#define kPMLastPageAEKey				'lwlp'
#define kPMLastPageAEType				typeSInt32

#define kPMLayoutAcrossAEProp				"pages across"
#define kPMLayoutAcrossAEKey				'lwla'
#define kPMLayoutAcrossAEType				typeSInt32

#define kPMLayoutDownAEProp				"pages down"
#define kPMLayoutDownAEKey				'lwld'
#define kPMLayoutDownAEType				typeSInt32

#define kPMErrorHandlingAEProp				"error handling"
#define kPMErrorHandlingAEKey				'lweh'
#define kPMErrorHandlingAEType				typeEnumerated

#define kPMPrintTimeAEProp				"requested print time"
#define kPMPrintTimeAEKey				'lwqt'
#define kPMPrintTimeAEType				cLongDateTime

#define kPMFeatureAEProp				"printer features"
#define kPMFeatureAEKey					'lwpf'
#define kPMFeatureAEType				typeAEList

#define	kPMFaxNumberAEProp				"fax number"
#define kPMFaxNumberAEKey				'faxn'
#define kPMFaxNumberAEType				typeChar

#define kPMTargetPrinterAEProp			"target printer"
#define kPMTargetPrinterAEKey			'trpr'
#define kPMTargetPrinterAEType			typeChar

/*** Enumerations ***/

/* For kPMErrorHandlingAEType */
#define kPMErrorHandlingStandardEnum		'lwst'
#define kPMErrorHandlingDetailedEnum		'lwdt'


#endif		// __PMPrintAETypes_R__
