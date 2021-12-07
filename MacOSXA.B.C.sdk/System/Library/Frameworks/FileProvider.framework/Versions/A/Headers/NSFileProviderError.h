//
//  NSFileProviderError.h
//  FileProvider
//
//  Copyright (c) 2014-2020 Apple Inc. All rights reserved.
//

#import <FileProvider/NSFileProviderDefines.h>
#import <FileProvider/NSFileProviderItem.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT NSErrorDomain const NSFileProviderErrorDomain FILEPROVIDER_API_AVAILABILITY_V2_V3;

FOUNDATION_EXPORT NSErrorUserInfoKey const NSFileProviderErrorCollidingItemKey API_DEPRECATED("NSFileProviderErrorItemKey", ios(8.0, 13.0)) API_UNAVAILABLE(macos, tvos, watchos);
FOUNDATION_EXPORT NSErrorUserInfoKey const NSFileProviderErrorItemKey API_AVAILABLE(ios(13.0), macos(10.15)) API_UNAVAILABLE(macCatalyst) API_UNAVAILABLE(watchos, tvos);
FOUNDATION_EXPORT NSErrorUserInfoKey const NSFileProviderErrorNonExistentItemIdentifierKey FILEPROVIDER_API_AVAILABILITY_V2_V3;

typedef NS_ERROR_ENUM(NSFileProviderErrorDomain, NSFileProviderErrorCode) {
    /** The user credentials cannot be verified */
    NSFileProviderErrorNotAuthenticated  = -1000,

    /** An item already exists with the same parentItemIdentifier and filename (or with a filename differing only in case.)

     \note Please use -[NSError (NSFileProviderError) fileProviderErrorForCollisionWithItem:] to build an error with this code.
     \see -[NSError (NSFileProviderError) fileProviderErrorForCollisionWithItem:]
     */
    NSFileProviderErrorFilenameCollision = -1001,

    /** The value of the sync anchor is too old, and the system must re-sync from scratch */
    NSFileProviderErrorSyncAnchorExpired = -1002,

    /** The value of the page token is too old, and the system must re-sync from scratch */
    NSFileProviderErrorPageExpired        = NSFileProviderErrorSyncAnchorExpired,

    /** The item has not been uploaded because it would push the account over quota */
    NSFileProviderErrorInsufficientQuota = -1003,

    /** Connecting to the servers failed */
    NSFileProviderErrorServerUnreachable = -1004,

    /** The requested item doesn't exist

     \note Please use -[NSError (NSFileProviderError) fileProviderErrorForNonExistentItemWithIdentifier:] to build an error with this code.
     \see -[NSError (NSFileProviderError) fileProviderErrorForNonExistentItemWithIdentifier:]
     */
    NSFileProviderErrorNoSuchItem        = -1005,

    /** The provider disallowed the deletion of the item.

     \note Please use -[NSError (NSFileProviderError) fileProviderErrorForRejectedDeletionOfItem:] to build an error with this code.
     \see -[NSError (NSFileProviderError) fileProviderErrorForRejectedDeletionOfItem:]
     */
    NSFileProviderErrorDeletionRejected FILEPROVIDER_API_AVAILABILITY_V3 = -1006,

    /** We're trying to non-recursively delete a non-empty directory
     */
    NSFileProviderErrorDirectoryNotEmpty FILEPROVIDER_API_AVAILABILITY_V3 = -1007,

    /**
     Returned by NSFileProviderManager if no provider could be found in the application
     */
    NSFileProviderErrorProviderNotFound FILEPROVIDER_API_AVAILABILITY_V3 = -2001,

    /**
     Returned by NSFileProviderManager if the application's provider has been disabled due to app translocation
     */
    NSFileProviderErrorProviderTranslocated FILEPROVIDER_API_AVAILABILITY_V3 = -2002,

    /**
     Returned by NSFileProviderManager if the provider registered in the system is an older version than the one corresponding to this app.
     */
    NSFileProviderErrorOlderExtensionVersionRunning FILEPROVIDER_API_AVAILABILITY_V3 = -2003,

    /**
     Returned by NSFileProviderManager if the provider registered in the system is a newer version than the one corresponding to this app.
     */
    NSFileProviderErrorNewerExtensionVersionFound FILEPROVIDER_API_AVAILABILITY_V3 = -2004,

    /**
     Returned by NSFileProviderManager if a barrier failed for a sync-related error.

     If the failure is caused by a specific item, the NSFileProviderErrorItemKey will be set to the corresponding item identifier
     and the NSUnderlyingErrorKey will be set to the error encountered by that item.
     */
    NSFileProviderErrorCannotSynchronize FILEPROVIDER_API_AVAILABILITY_V3 = -2005
} FILEPROVIDER_API_AVAILABILITY_V2_V3;

@interface NSError (NSFileProviderError)
+ (instancetype)fileProviderErrorForCollisionWithItem:(NSFileProviderItem)existingItem FILEPROVIDER_API_AVAILABILITY_V2_V3;
+ (instancetype)fileProviderErrorForNonExistentItemWithIdentifier:(NSFileProviderItemIdentifier)itemIdentifier FILEPROVIDER_API_AVAILABILITY_V2_V3;
+ (instancetype)fileProviderErrorForRejectedDeletionOfItem:(NSFileProviderItem)updatedVersion NS_SWIFT_NAME(fileProviderErrorForRejectedDeletion(of:)) FILEPROVIDER_API_AVAILABILITY_V3;

@end

NS_ASSUME_NONNULL_END
