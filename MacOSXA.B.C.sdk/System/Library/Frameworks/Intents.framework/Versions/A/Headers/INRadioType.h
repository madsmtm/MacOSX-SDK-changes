//
//  INRadioType.h
//  Intents
//
//  Copyright (c) 2016-2021 Apple Inc. All rights reserved.
//

#ifndef INRadioType_h
#define INRadioType_h

#import <Foundation/Foundation.h>
#import <Intents/IntentsDefines.h>

typedef NS_ENUM(NSInteger, INRadioType) {
    INRadioTypeUnknown = 0,
    INRadioTypeAM,
    INRadioTypeFM,
    INRadioTypeHD,
    INRadioTypeSatellite,
    INRadioTypeDAB,
} API_AVAILABLE(ios(10.0)) API_UNAVAILABLE(macos, tvos) API_UNAVAILABLE(watchos);

#endif // INRadioType_h
