/*
 * Copyright (c) 2014, 2015 Apple Inc.
 * All rights reserved.
 */

#ifndef __NE_INDIRECT__
#define __NE_INDIRECT__
#endif

#import <Foundation/Foundation.h>

#import <NetworkExtension/NEAppProxyFlow.h>
#import <NetworkExtension/NEAppProxyProvider.h>
#import <NetworkExtension/NEAppProxyProviderManager.h>
#import <NetworkExtension/NEAppProxyTCPFlow.h>
#import <NetworkExtension/NEAppProxyUDPFlow.h>
#import <NetworkExtension/NEAppRule.h>
#import <NetworkExtension/NEDNSSettings.h>
#import <NetworkExtension/NEFilterControlProvider.h>
#import <NetworkExtension/NEFilterDataProvider.h>
#import <NetworkExtension/NEFilterFlow.h>
#import <NetworkExtension/NEFilterManager.h>
#import <NetworkExtension/NEFilterProvider.h>
#import <NetworkExtension/NEFilterProviderConfiguration.h>
#if TARGET_OS_IPHONE
#import <NetworkExtension/NEHotspotHelper.h>
#endif
#import <NetworkExtension/NEIPv4Settings.h>
#import <NetworkExtension/NEIPv6Settings.h>
#import <NetworkExtension/NEOnDemandRule.h>
#import <NetworkExtension/NEPacketTunnelFlow.h>
#import <NetworkExtension/NEPacketTunnelNetworkSettings.h>
#import <NetworkExtension/NEPacketTunnelProvider.h>
#import <NetworkExtension/NEProvider.h>
#import <NetworkExtension/NEProxySettings.h>
#import <NetworkExtension/NETunnelProvider.h>
#import <NetworkExtension/NETunnelProviderManager.h>
#import <NetworkExtension/NETunnelProviderSession.h>
#import <NetworkExtension/NETunnelProviderProtocol.h>
#import <NetworkExtension/NEVPNConnection.h>
#import <NetworkExtension/NEVPNManager.h>
#import <NetworkExtension/NEVPNProtocol.h>
#import <NetworkExtension/NEVPNProtocolIKEv2.h>
#import <NetworkExtension/NEVPNProtocolIPSec.h>

#import <NetworkExtension/NWEndpoint.h>
#import <NetworkExtension/NWHostEndpoint.h>
#import <NetworkExtension/NWBonjourServiceEndpoint.h>
#import <NetworkExtension/NWPath.h>
#import <NetworkExtension/NWTCPConnection.h>
#import <NetworkExtension/NWUDPSession.h>
#import <NetworkExtension/NWTLSParameters.h>

#undef __NE_INDIRECT__