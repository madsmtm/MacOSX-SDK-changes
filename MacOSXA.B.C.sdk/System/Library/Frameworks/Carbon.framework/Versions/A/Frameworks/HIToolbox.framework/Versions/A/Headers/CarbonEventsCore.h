/*
     File:       HIToolbox/CarbonEventsCore.h
 
     Contains:   Carbon Event Manager
 
     Version:    HIToolbox-145.14~1
 
     Copyright:  © 1999-2003 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CARBONEVENTSCORE__
#define __CARBONEVENTSCORE__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif


#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma options align=mac68k

/*======================================================================================*/
/*  The core data structure of the Carbon Event system                                  */
/*======================================================================================*/
typedef struct OpaqueEventRef*          EventRef;
/*======================================================================================*/
/*  EVENT COMMON                                                                        */
/*======================================================================================*/


/*
 *  Discussion:
 *    The following are all errors which can be returned from the
 *    routines contained in this file.
 */
enum {

  /*
   * This is returned from PostEventToQueue if the event in question is
   * already in the queue you are posting it to (or any other queue).
   */
  eventAlreadyPostedErr         = -9860,

  /*
   * You are attemtping to modify a target that is currently in use,
   * such as when dispatching.
   */
  eventTargetBusyErr            = -9861,

  /*
   * This is obsolete and will be removed.
   */
  eventClassInvalidErr          = -9862,

  /*
   * This is obsolete and will be removed.
   */
  eventClassIncorrectErr        = -9864,

  /*
   * This is what you should return from a kEventClassAccessibility
   * event handler when asked to process a directly dispached event
   * that would cause your handler proc to not return after more than a
   * split-second. This will cause the accessibility engine to defer
   * the event until a later time when your handler will be able to
   * take all the time it needs without causing timeout problems for
   * the assistive application. See kEventClassAccessibility and
   * kEventAccessiblePerformNamedAction for more information. You
   * should only return this on Mac OS X 10.3 and later; earlier
   * versions will treat this like a true failure, which prevents
   * assistive applications from getting the functionality they need.
   */
  eventDeferAccessibilityEventErr = -9865,

  /*
   * Returned from InstallEventHandler if the handler proc you pass is
   * already installed for a given event type you are trying to
   * register.
   */
  eventHandlerAlreadyInstalledErr = -9866,

  /*
   * A generic error.
   */
  eventInternalErr              = -9868,

  /*
   * This is obsolete and will be removed.
   */
  eventKindIncorrectErr         = -9869,

  /*
   * The piece of data you are requesting from an event is not present.
   */
  eventParameterNotFoundErr     = -9870,

  /*
   * This is what you should return from an event handler when your
   * handler has received an event it doesn't currently want to (or
   * isn't able to) handle. If you handle an event, you should return
   * noErr from your event handler. Any return value other than
   * eventNotHandledErr will cause event handling to stop; the event
   * will not be sent to any other event handler, and the return value
   * will be provided to the original caller of SendEventToTarget.
   */
  eventNotHandledErr            = -9874,

  /*
   * The event loop has timed out. This can be returned from calls to
   * ReceiveNextEvent or RunCurrentEventLoop.
   */
  eventLoopTimedOutErr          = -9875,

  /*
   * The event loop was quit, probably by a call to QuitEventLoop. This
   * can be returned from ReceiveNextEvent or RunCurrentEventLoop.
   */
  eventLoopQuitErr              = -9876,

  /*
   * Returned from RemoveEventFromQueue when trying to remove an event
   * that's not in any queue.
   */
  eventNotInQueueErr            = -9877,

  /*
   * Returned from RegisterEventHotKey when an attempt is made to
   * register a hotkey that is already registered in the current
   * process. (Note that it is not an error to register the same hotkey
   * in multiple processes.)
   */
  eventHotKeyExistsErr          = -9878,

  /*
   * This error code is not currently used.
   */
  eventHotKeyInvalidErr         = -9879,

  /*
   * When returned from an event handler, causes the event dispatcher
   * to abandon event dispatching on this target, and pass the event to
   * the first handler on the next event target. Any event handlers
   * installed beneath the current handler on the current target will
   * not receive the event. Although newly documented in Mac OS X 10.3,
   * this error code is actually available on Mac OS X 10.0 and
   * CarbonLib 1.3 and later.
   */
  eventPassToNextTargetErr      = -9880
};

/*======================================================================================*/
/*  EVENT CORE                                                                          */
/*======================================================================================*/
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ Core Event Parameters                                                             */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
enum {
  kEventParamDirectObject       = '----', /* type varies depending on event*/
  kEventParamDragRef            = 'drag' /* typeDragRef*/
};

/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ Core Event Types                                                                  */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*
    Some Carbon event data types will be retained when added to an EventRef in
    Mac OS X 10.2 and later, and released when the EventRef is destroyed.
    
        Retained in 10.2 and later:
        
            typeCFStringRef
            typeCFMutableStringRef
            typeCFMutableArrayRef
            typeCFTypeRef
            typeHIAccessibleObjectRef
            
        Retained in Panther and later:
        
            typeCFArrayRef
            typeCFDictionaryRef:
            typeCFMutableDictionaryRef
            
    Note that other data types may be retained in future releases of Mac OS X.
    Apple recommends that if you need to know whether a particular data type
    (other than the ones documented here) is retained, that you check the retain
    count of an instance of that data type before and after adding it to an EventRef.
*/
enum {
  typeCFStringRef               = 'cfst', /* CFStringRef*/
  typeCFMutableStringRef        = 'cfms', /* CFMutableStringRef*/
  typeCFArrayRef                = 'cfar', /* CFArrayRef*/
  typeCFMutableArrayRef         = 'cfma', /* CFMutableArrayRef*/
  typeCFDictionaryRef           = 'cfdc', /* CFDictionaryRef*/
  typeCFMutableDictionaryRef    = 'cfmd', /* CFMutableDictionaryRef*/
  typeCFTypeRef                 = 'cfty', /* CFTypeRef*/
  typeDragRef                   = 'drag' /* DragRef*/
};

/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ Event Flags, options                                                              */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/

/*
 *  EventPriority
 *  
 *  Discussion:
 *    These values define the relative priority of an event, and are
 *    used when posting events with PostEventToQueue. In general events
 *    are pulled from the queue in order of first posted to last
 *    posted. These priorities are a way to alter that when posting
 *    events. You can post a standard priority event and then a high
 *    priority event and the high priority event will be pulled from
 *    the queue first.
 */
typedef SInt16 EventPriority;
enum {

  /*
   * Lowest priority. Currently only window update events are posted at
   * this priority.
   */
  kEventPriorityLow             = 0,

  /*
   * Normal priority of events. Most events are standard priority.
   */
  kEventPriorityStandard        = 1,

  /*
   * Highest priority.
   */
  kEventPriorityHigh            = 2
};

enum {
  kEventLeaveInQueue            = false,
  kEventRemoveFromQueue         = true
};

/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/* ₯ Event Times                                                                        */
/*                                                                                      */
/* EventTime is in seconds since boot. Use the constants to make life easy.             */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
typedef double                          EventTime;
typedef EventTime                       EventTimeout;
typedef EventTime                       EventTimerInterval;
#define kEventDurationSecond            ((EventTime)1.0)
#define kEventDurationMillisecond       ((EventTime)(kEventDurationSecond/1000))
#define kEventDurationMicrosecond       ((EventTime)(kEventDurationSecond/1000000))
#define kEventDurationNanosecond        ((EventTime)(kEventDurationSecond/1000000000))
#define kEventDurationMinute            ((EventTime)(kEventDurationSecond*60))
#define kEventDurationHour              ((EventTime)(kEventDurationMinute*60))
#define kEventDurationDay               ((EventTime)(kEventDurationHour*24))
#define kEventDurationNoWait            ((EventTime)0.0)
#define kEventDurationForever           ((EventTime)(-1.0))

/* Helpful doodads to convert to and from ticks and event times*/
#ifdef __cplusplus
    inline EventTime TicksToEventTime( UInt32 t ) { return ( (t) / 60.0 ); }
    inline UInt32 EventTimeToTicks( EventTime t ) { return (UInt32)( ((t) * 60) + 0.5 ); }
#else
    #define TicksToEventTime( t )   ((EventTime)( (t) / 60.0 ))
    #define EventTimeToTicks( t )   ((UInt32)( ((t) * 60) + 0.5 ))
#endif  /* defined(__cplusplus) */


/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/* EventTypeSpec structure                                                              */
/*                                                                                      */
/* This structure is used in many routines to pass a list of event types to a function. */
/* You typically would declare a const array of these types to pass in.                 */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/

/*
 *  EventTypeSpec
 *  
 *  Discussion:
 *    This structure is used to specify an event. Typically, a static
 *    array of EventTypeSpecs are passed into functions such as
 *    InstallEventHandler, as well as routines such as
 *    FlushEventsMatchingListFromQueue.
 */
struct EventTypeSpec {
  UInt32              eventClass;
  UInt32              eventKind;
};
typedef struct EventTypeSpec            EventTypeSpec;
/*A helpful macro for dealing with EventTypeSpecs */
#define GetEventTypeCount( t )  (sizeof( (t) ) / sizeof( EventTypeSpec ))

typedef OSType                          EventParamName;
typedef OSType                          EventParamType;
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ EventLoop                                                                         */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/

/*
 *  EventLoopRef
 *  
 *  Discussion:
 *    An EventLoopRef represents an 'event loop', which is the
 *    conceptual entity that you 'run' to fetch events from hardware
 *    and other sources and also fires timers that might be installed
 *    with InstallEventLoopTimer. The term 'run' is a bit of a
 *    misnomer, as the event loop's goal is to stay as blocked as
 *    possible to minimize CPU usage for the current application. The
 *    event loop is run implicitly thru APIs like ReceiveNextEvent,
 *    RunApplicationEventLoop, or even WaitNextEvent. It can also be
 *    run explicitly thru a call to RunCurrentEventLoop. Each
 *    preemptive thread can have an event loop. Cooperative threads
 *    share the main thread's event loop.
 */
typedef struct OpaqueEventLoopRef*      EventLoopRef;
/*
 *  GetCurrentEventLoop()
 *  
 *  Discussion:
 *    Returns the current event loop for the current thread. If the
 *    current thread is a cooperative thread, the main event loop is
 *    returned.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    An event loop reference.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern EventLoopRef 
GetCurrentEventLoop(void)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetMainEventLoop()
 *  
 *  Discussion:
 *    Returns the event loop object for the main application thread.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    An event loop reference.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern EventLoopRef 
GetMainEventLoop(void)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  RunCurrentEventLoop()
 *  
 *  Discussion:
 *    This routine 'runs' the event loop, returning only if aborted or
 *    the timeout specified is reached. The event loop is mostly
 *    blocked while in this function, occasionally waking up to fire
 *    timers or pick up events. The typical use of this function is to
 *    cause the current thread to wait for some operation to complete,
 *    most likely on another thread of execution.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inTimeout:
 *      The time to wait until returning (can be kEventDurationForever).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
RunCurrentEventLoop(EventTimeout inTimeout)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  QuitEventLoop()
 *  
 *  Discussion:
 *    Causes a specific event loop to terminate. Usage of this is
 *    similar to WakeUpProcess, in that it causes the eventloop
 *    specified to return immediately (as opposed to timing out).
 *    Typically this call is used in conjunction with
 *    RunCurrentEventLoop.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEventLoop:
 *      The event loop to terminate.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
QuitEventLoop(EventLoopRef inEventLoop)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetCFRunLoopFromEventLoop()
 *  
 *  Discussion:
 *    Returns the corresponding CFRunLoopRef for the given EventLoop.
 *    This is not necessarily a one-to-one mapping, hence the need for
 *    this function. In Carbon, all cooperative threads use the same
 *    run loop under the covers, so using CFRunLoopGetCurrent might
 *    yield the wrong result. In general, you would only need to use
 *    this function if you wished to add your own sources to the run
 *    loop. If you don't know what I'm talking about, then you probably
 *    don't need to use this.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEventLoop:
 *      The event loop to get the CFRunLoop for.
 *  
 *  Result:
 *    The CFRunLoopRef for inEventLoop.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.1 and later
 *    Non-Carbon CFM:   not available
 */
extern CFTypeRef 
GetCFRunLoopFromEventLoop(EventLoopRef inEventLoop)           AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;


/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ Low-level event fetching                                                          */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*
 *  ReceiveNextEvent()
 *  
 *  Discussion:
 *    This routine tries to fetch the next event of a specified type.
 *    If no events in the event queue match, this routine will run the
 *    current event loop until an event that matches arrives, or the
 *    timeout expires. Except for timers firing, your application is
 *    blocked waiting for events to arrive when inside this function.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inNumTypes:
 *      The number of event types we are waiting for (0 if any event
 *      should cause this routine to return).
 *    
 *    inList:
 *      The list of event types we are waiting for (pass NULL if any
 *      event should cause this routine to return).
 *    
 *    inTimeout:
 *      The time to wait (passing kEventDurationForever is preferred).
 *    
 *    inPullEvent:
 *      Pass true for this parameter to actually remove the next
 *      matching event from the queue.
 *    
 *    outEvent:
 *      The next event that matches the list passed in. If inPullEvent
 *      is true, the event is owned by you, and you will need to
 *      release it when done.
 *  
 *  Result:
 *    A result indicating whether an event was received, the timeout
 *    expired, or the current event loop was quit.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
ReceiveNextEvent(
  UInt32                 inNumTypes,
  const EventTypeSpec *  inList,
  EventTimeout           inTimeout,
  Boolean                inPullEvent,
  EventRef *             outEvent)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ Core event lifetime APIs                                                          */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/

/*
 *  EventAttributes
 *  
 *  Summary:
 *    Attributes of a Carbon event.
 */
typedef UInt32 EventAttributes;
enum {

  /*
   * The absence of any attributes.
   */
  kEventAttributeNone           = 0,

  /*
   * Indicates that the event is considered user input; for example, a
   * mouse event or keyboard event. Not appropriate for higher-level
   * events such as a window update or activate.
   */
  kEventAttributeUserEvent      = 1 << 0,

  /*
   * This event was not originally targeted to this process, but has
   * been provided to this process because someone has installed an
   * event handler for this event type on the event monitoring target.
   * Events with this attribute are sent directly to the event monitor
   * target by the event dispatcher.
   */
  kEventAttributeMonitored      = 1 << 3
};

/*
 *  [Mac]CreateEvent()
 *  
 *  Summary:
 *    Creates a new Carbon event.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inAllocator:
 *      The CFAllocator to use to allocate the event data. You can pass
 *      NULL or kCFAllocatorDefault to use the standard allocator.
 *    
 *    inClassID:
 *      The event class for the event.
 *    
 *    inKind:
 *      The event kind for the event.
 *    
 *    inWhen:
 *      The event timestamp to be recorded in the event. You may pass 0
 *      to indicate the current time.
 *    
 *    inAttributes:
 *      The event attributes. Typically this should be
 *      kEventAttributeNone.
 *    
 *    outEvent:
 *      On exit, contains the new event.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
#if TARGET_OS_MAC
    #define MacCreateEvent CreateEvent
#endif
extern OSStatus 
MacCreateEvent(
  CFAllocatorRef    inAllocator,        /* can be NULL */
  UInt32            inClassID,
  UInt32            inKind,
  EventTime         inWhen,
  EventAttributes   inAttributes,
  EventRef *        outEvent)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CopyEvent()
 *  
 *  Summary:
 *    Copies an existing event.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inOther:
 *      The original event to copy.
 *  
 *  Result:
 *    The newly created event, or NULL if either the input event was
 *    NULL or the memory for the event could not be allocated. The new
 *    event is allocated using the same allocator as the original event.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern EventRef 
CopyEvent(EventRef inOther)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CopyEventAs()
 *  
 *  Summary:
 *    Copies an existing event, allowing you to change the class and
 *    kind of the event.
 *  
 *  Discussion:
 *    CopyEventAs may be useful during event flow and transformation.
 *    For example, this API is used when upgrading a raw mouse down to
 *    a window click event, to ensure that the window click event has
 *    exactly the same parameters as the original mouse down event.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inAllocator:
 *      The CFAllocator to use to allocate the event data. You can pass
 *      NULL or kCFAllocatorDefault to use the standard allocator.
 *    
 *    inOther:
 *      The original event to copy.
 *    
 *    inEventClass:
 *      The new event class for the copy of the event.
 *    
 *    inEventKind:
 *      The new event kind for the copy of the event.
 *  
 *  Result:
 *    The newly created event, or NULL if either the input event was
 *    NULL or the memory for the event could not be allocated.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern EventRef 
CopyEventAs(
  CFAllocatorRef   inAllocator,        /* can be NULL */
  EventRef         inOther,
  UInt32           inEventClass,
  UInt32           inEventKind)                               AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  RetainEvent()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern EventRef 
RetainEvent(EventRef inEvent)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetEventRetainCount()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern UInt32 
GetEventRetainCount(EventRef inEvent)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ReleaseEvent()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
ReleaseEvent(EventRef inEvent)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetEventParameter()
 *  
 *  Discussion:
 *    Sets a piece of data for the given event.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEvent:
 *      The event to set the data for.
 *    
 *    inName:
 *      The symbolic name of the parameter.
 *    
 *    inType:
 *      The symbolic type of the parameter.
 *    
 *    inSize:
 *      The size of the parameter data.
 *    
 *    inDataPtr:
 *      The pointer to the parameter data.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetEventParameter(
  EventRef         inEvent,
  EventParamName   inName,
  EventParamType   inType,
  UInt32           inSize,
  const void *     inDataPtr)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  GetEventParameter()
 *  
 *  Discussion:
 *    Gets a piece of data from the given event, if it exists.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEvent:
 *      The event to get the parameter from.
 *    
 *    inName:
 *      The symbolic name of the parameter.
 *    
 *    inDesiredType:
 *      The desired type of the parameter. The Carbon Event Manager
 *      will automatically use AppleEvent coercion handlers to convert
 *      the data in the event into the desired type, if possible. You
 *      may also pass typeWildCard to request that the data be returned
 *      in its original format.
 *    
 *    outActualType:
 *      The actual type of the parameter, or NULL if you are not
 *      interested in receiving this information.
 *    
 *    inBufferSize:
 *      The size of the output buffer specified by ioBuffer. You may
 *      pass zero for this parameter and NULL for the outData parameter
 *      if you don't want the data returned.
 *    
 *    outActualSize:
 *      The actual size of the data, or NULL if you don't want this
 *      information.
 *    
 *    outData:
 *      The pointer to the buffer which will receive the parameter
 *      data, or NULL if you don't want the data returned. If you pass
 *      NULL, you must also pass zero for the inBufferSize parameter.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetEventParameter(
  EventRef          inEvent,
  EventParamName    inName,
  EventParamType    inDesiredType,
  EventParamType *  outActualType,       /* can be NULL */
  UInt32            inBufferSize,
  UInt32 *          outActualSize,       /* can be NULL */
  void *            outData)             /* can be NULL */    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ Getters for 'base-class' event info                                               */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*
 *  GetEventClass()
 *  
 *  Discussion:
 *    Returns the class of the given event, such as mouse, keyboard,
 *    etc.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEvent:
 *      The event in question.
 *  
 *  Result:
 *    The class ID of the event.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern UInt32 
GetEventClass(EventRef inEvent)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetEventKind()
 *  
 *  Discussion:
 *    Returns the kind of the given event (mousedown, etc.). Event
 *    kinds overlap between event classes, e.g. kEventMouseDown and
 *    kEventAppActivated have the same value (1). The combination of
 *    class and kind is what determines an event signature.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEvent:
 *      The event in question.
 *  
 *  Result:
 *    The kind of the event.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern UInt32 
GetEventKind(EventRef inEvent)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetEventTime()
 *  
 *  Discussion:
 *    Returns the time the event specified occurred, specified in
 *    EventTime, which is a floating point number representing seconds
 *    since the last system startup.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEvent:
 *      The event in question.
 *  
 *  Result:
 *    The time the event occurred.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern EventTime 
GetEventTime(EventRef inEvent)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ Setters for 'base-class' event info                                               */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/

/*
 *  SetEventTime()
 *  
 *  Discussion:
 *    This routine allows you to set the time of a given event, if you
 *    so desire. In general, you would never use this routine, except
 *    for those special cases where you reuse an event from time to
 *    time instead of creating a new event each time.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEvent:
 *      The event in question.
 *    
 *    inTime:
 *      The new time.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetEventTime(
  EventRef    inEvent,
  EventTime   inTime)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ Event Queue routines (posting, finding, flushing)                                 */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/

typedef struct OpaqueEventQueueRef*     EventQueueRef;
/*
 *  GetCurrentEventQueue()
 *  
 *  Discussion:
 *    Returns the current event queue for the current thread. If the
 *    current thread is a cooperative thread, the main event queue is
 *    returned.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    An event queue reference.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern EventQueueRef 
GetCurrentEventQueue(void)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetMainEventQueue()
 *  
 *  Discussion:
 *    Returns the event queue object for the main application thread.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    An event queue reference.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern EventQueueRef 
GetMainEventQueue(void)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  EventComparatorProcPtr
 *  
 *  Discussion:
 *    Type of a callback function used by queue searches.
 *  
 *  Parameters:
 *    
 *    inEvent:
 *      The event to compare.
 *    
 *    inCompareData:
 *      The data used to compare the event.
 *  
 *  Result:
 *    A boolean value indicating whether the event matches (true) or
 *    not (false).
 */
typedef CALLBACK_API( Boolean , EventComparatorProcPtr )(EventRef inEvent, void *inCompareData);
typedef STACK_UPP_TYPE(EventComparatorProcPtr)                  EventComparatorUPP;
/*
 *  NewEventComparatorUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern EventComparatorUPP
NewEventComparatorUPP(EventComparatorProcPtr userRoutine)     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeEventComparatorUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeEventComparatorUPP(EventComparatorUPP userUPP)         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeEventComparatorUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern Boolean
InvokeEventComparatorUPP(
  EventRef            inEvent,
  void *              inCompareData,
  EventComparatorUPP  userUPP)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  PostEventToQueue()
 *  
 *  Discussion:
 *    Posts an event to the queue specified. This automatically wakes
 *    up the event loop of the thread the queue belongs to. After
 *    posting the event, you should release the event. The event queue
 *    retains it.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    inQueue:
 *      The event queue to post the event onto.
 *    
 *    inEvent:
 *      The event to post.
 *    
 *    inPriority:
 *      The priority of the event.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
PostEventToQueue(
  EventQueueRef   inQueue,
  EventRef        inEvent,
  EventPriority   inPriority)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  FlushEventsMatchingListFromQueue()
 *  
 *  Discussion:
 *    Flushes events matching a specified list of classes and kinds
 *    from an event queue.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    inQueue:
 *      The event queue to flush events from.
 *    
 *    inNumTypes:
 *      The number of event kinds to flush.
 *    
 *    inList:
 *      The list of event classes and kinds to flush from the queue.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
FlushEventsMatchingListFromQueue(
  EventQueueRef          inQueue,
  UInt32                 inNumTypes,
  const EventTypeSpec *  inList)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  FlushSpecificEventsFromQueue()
 *  
 *  Discussion:
 *    Flushes events that match a comparator function.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    inQueue:
 *      The event queue to flush events from.
 *    
 *    inComparator:
 *      The comparison function to invoke for each event in the queue.
 *    
 *    inCompareData:
 *      The data you wish to pass to your comparison function.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
FlushSpecificEventsFromQueue(
  EventQueueRef        inQueue,
  EventComparatorUPP   inComparator,
  void *               inCompareData)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  FlushEventQueue()
 *  
 *  Discussion:
 *    Flushes all events from an event queue.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    inQueue:
 *      The event queue to flush.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
FlushEventQueue(EventQueueRef inQueue)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  FindSpecificEventInQueue()
 *  
 *  Discussion:
 *    Returns the first event that matches a comparator function, or
 *    NULL if no events match.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    inQueue:
 *      The event queue to search.
 *    
 *    inComparator:
 *      The comparison function to invoke for each event in the queue.
 *    
 *    inCompareData:
 *      The data you wish to pass to your comparison function.
 *  
 *  Result:
 *    An event reference. The event is still in the queue when
 *    FindSpecificEventInQueue returns; you can remove it from the
 *    queue with RemoveEventFromQueue. The returned event does not need
 *    to be released by the caller.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern EventRef 
FindSpecificEventInQueue(
  EventQueueRef        inQueue,
  EventComparatorUPP   inComparator,
  void *               inCompareData)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetNumEventsInQueue()
 *  
 *  Discussion:
 *    Returns the number of events in an event queue.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    inQueue:
 *      The event queue to query.
 *  
 *  Result:
 *    The number of items in the queue.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern UInt32 
GetNumEventsInQueue(EventQueueRef inQueue)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  RemoveEventFromQueue()
 *  
 *  Discussion:
 *    Removes the given event from the queue which it was posted. When
 *    you call this function, the event ownership is transferred to
 *    you, the caller, at no charge. You must release the event when
 *    you are through with it.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    inQueue:
 *      The queue to remove the event from.
 *    
 *    inEvent:
 *      The event to remove.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
RemoveEventFromQueue(
  EventQueueRef   inQueue,
  EventRef        inEvent)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  IsEventInQueue()
 *  
 *  Discussion:
 *    Returns true if the specified event is posted to a queue.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inQueue:
 *      The queue to check.
 *    
 *    inEvent:
 *      The event in question.
 *  
 *  Result:
 *    A boolean value.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
IsEventInQueue(
  EventQueueRef   inQueue,
  EventRef        inEvent)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 */
enum {

  /*
   * Currently, the only value you can pass to
   * AcquireFirstMatchingEventInQueue in its inOptions parameter.
   */
  kEventQueueOptionsNone        = 0
};

/*
 *  AcquireFirstMatchingEventInQueue()
 *  
 *  Discussion:
 *    Returns the first event that matches the list of event classes
 *    and kinds passed in. This call does not call the event loop, and
 *    hence no timers will fire nor will any window flushing occur when
 *    this API is called. New events will be pulled from the window
 *    server, however. Overall this API should have better performance
 *    characteristics than the older EventAvail API.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inQueue:
 *      The queue to check.
 *    
 *    inNumTypes:
 *      The number of event kinds to search for. You may pass zero for
 *      this parameter if you also pass NULL for inList.
 *    
 *    inList:
 *      The list of event classes and kinds to search for in the queue.
 *      You may pass NULL for this parameter if you also pass zero for
 *      inNumTypes. This effectively matches ANY event in the queue,
 *      and will merely return the first event in the queue.
 *    
 *    inOptions:
 *      Currently, you must pass kEventQueueOptionsNone for this
 *      parameter.
 *  
 *  Result:
 *    An event reference, or NULL if no events match the list passed.
 *    The event returned has had its refcount incremented (i.e. it has
 *    been retained). As a result, you must release this value
 *    (assuming it's non-NULL). The event is not removed from the queue
 *    by this API; you should call RemoveEventFromQueue if necessary.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern EventRef 
AcquireFirstMatchingEventInQueue(
  EventQueueRef          inQueue,
  UInt32                 inNumTypes,
  const EventTypeSpec *  inList,
  OptionBits             inOptions)                           AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  Queue-synchronized event state                                                      */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*
 *  GetCurrentEvent()
 *  
 *  Summary:
 *    Returns the user input event currently being handled.
 *  
 *  Discussion:
 *    When an event with kEventAttributeUserEvent is dispatched by the
 *    event dispatcher target, it is recorded internally by the Event
 *    Manager. At any time during the handling of that event,
 *    GetCurrentEvent may be used to retrieve the original EventRef.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    The user input (mouse or keyboard) event currently being handled.
 *    May be NULL if no event is currently being handled, or if the
 *    current event was not a user input event. The returned event is
 *    not retained, and its lifetime should be considered to be no
 *    longer than the current function; if you need to keep the event
 *    alive past that time, you should retain it.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern EventRef 
GetCurrentEvent(void)                                         AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  GetCurrentEventButtonState()
 *  
 *  Summary:
 *    Returns the current queue-synchronized mouse button state on the
 *    primary input device.
 *  
 *  Discussion:
 *    At any point in the handling of user input, there are two
 *    different mouse button states: the queue-synchronized state and
 *    the hardware state. The hardware state reflects the actual
 *    current state of the mouse attached to the user's machine. The
 *    queue-synchronized state reflects the state according to the
 *    events that have been processed at that point by the application.
 *    These two states may be different if there are unprocessed events
 *    in the event queue, or if events are being artificially
 *    introduced into the event queue from an outside source.
 *    GetCurrentEventButtonState returns the queue-synchronized button
 *    state. It is generally better to use this API than to use the
 *    Button function or the GetCurrentButtonState function (which
 *    return the hardware state). This gives a more consistent user
 *    experience when the user input queue is being remoted controlled
 *    or manipulated via non-hardware event sources such as speech or
 *    AppleEvents; using GetCurrentEventButtonState is also much faster
 *    than using Button or GetCurrentButtonState.
 *    
 *    Note that GetCurrentEventButtonState only returns a valid button
 *    state if your application is the active application. If your
 *    application is not active, then user input events are not flowing
 *    through the event dispatcher and the queue-synchronized state is
 *    not updated.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    The queue-synchronized state of the mouse buttons. Bit zero
 *    indicates the state of the primary button, bit one the state of
 *    the secondary button, and so on.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern UInt32 
GetCurrentEventButtonState(void)                              AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  GetCurrentEventKeyModifiers()
 *  
 *  Summary:
 *    Returns the current queue-synchronized keyboard modifier state.
 *  
 *  Discussion:
 *    At any point in the handling of user input, there are two
 *    different keyboard modifier states: the queue-synchronized state
 *    and the hardware state. The hardware state reflects the actual
 *    current state of the keyboard attached to the user's machine. The
 *    queue-synchronized state reflects the state according to the
 *    events that have been processed at that point by the application.
 *    These two states may be different if there are unprocessed events
 *    in the event queue, or if events are being artificially
 *    introduced into the event queue from an outside source.
 *    GetCurrentEventKeyModifiers returns the queue-synchronized
 *    modifier state. It is generally better to use this API than to
 *    use the GetCurrentKeyModifiers API (which returns the hardware
 *    state). This gives a more consistent user experience when the
 *    user input queue is being remoted controlled or manipulated via
 *    non-hardware event sources such as speech or AppleEvents; using
 *    GetCurrentEventKeyModifiers is also much faster than using
 *    EventAvail(0, &eventRecord) or GetCurrentKeyModifiers.
 *    
 *    Note that GetCurrentEventKeyModifiers only returns a valid
 *    modifier state if your application is the active application. If
 *    your application is not active, then user input events are not
 *    flowing through the event dispatcher and the queue-synchronized
 *    state is not updated.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    The queue-synchronized state of the keyboard modifiers. The
 *    format of the return value is the same as the modifiers field of
 *    an EventRecord (but only includes keyboard modifiers and not the
 *    other modifier flags included in an EventRecord).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern UInt32 
GetCurrentEventKeyModifiers(void)                             AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  Multiple-button support                                                             */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*
 *  GetCurrentButtonState()
 *  
 *  Summary:
 *    Returns the current hardware mouse button state on the primary
 *    input device.
 *  
 *  Discussion:
 *    In most cases, you should not use GetCurrentButtonState, but
 *    should use the GetCurrentEventButtonState function instead.
 *    GetCurrentEventButtonState is much faster than
 *    GetCurrentButtonState because it returns the locally cached
 *    button state; GetCurrentButtonState must get the mouse button
 *    state from the window server, which is slower. Using
 *    GetCurrentButtonState also can prevent your application from
 *    being operated by remote posting of events, since the hardware
 *    input device is not actually changing state in that case. Most
 *    commonly, you might need to use GetCurrentButtonState when your
 *    application is not the active application (as determined by the
 *    Process Manager function GetFrontProcess). In that case, the
 *    cached button state returned by GetCurrentEventButtonState is not
 *    valid because mouse button events are not flowing to your
 *    application, and you must use GetCurrentButtonState to determine
 *    the current hardware state.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    The state of the mouse buttons on the mouse hardware. Bit zero
 *    indicates the state of the primary button, bit one the state of
 *    the secondary button, and so on.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern UInt32 
GetCurrentButtonState(void)                                   AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ Helpful utilities                                                                 */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/

/*
 *  GetCurrentEventTime()
 *  
 *  Discussion:
 *    Returns the current time since last system startup in seconds.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    EventTime.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern EventTime 
GetCurrentEventTime(void)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ Timers                                                                            */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/

/*
 *  EventLoopTimerRef
 *  
 *  Discussion:
 *    An EventLoopTimerRef represents what we term a 'timer'. A timer
 *    is a function that is called either once or at regular intervals.
 *    It executes at task level and should not be confused with Time
 *    Manager Tasks or any other interrupt-level callback. This means
 *    you can call Toolbox routines, allocate memory and draw. When a
 *    timer 'fires', it calls a callback that you specify when the
 *    timer is installed. Timers in general have two uses - as a
 *    timeout mechanism and as a periodic task. An everyday example of
 *    using a timer for a timeout might be a light that goes out if no
 *    motion is detected in a room for 5 minutes. For this, you might
 *    install a timer which will fire in 5 minutes. If motion is
 *    detected, you would reset the timer fire time and let the clock
 *    start over. If no motion is detected for the full 5 minutes, the
 *    timer will fire and you could power off the light. A periodic
 *    timer is one that fires at regular intervals (say every second or
 *    so). You might use such a timer to blink the insertion point in
 *    your editor, etc. One advantage of timers is that you can install
 *    the timer right from the code that wants the time. For example,
 *    the standard Toolbox Edit Text control can install a timer to
 *    blink the cursor when it's active, meaning that IdleControls is a
 *    no-op for that control and doesn't need to be called. When the
 *    control is inactive, it removes its timer and doesn't waste CPU
 *    time in that state. NOTE: Currently, if you do decide to draw
 *    when your timer is called, be sure to save and restore the
 *    current port so that calling your timer doesn't inadvertently
 *    change the port out from under someone.
 */
typedef struct __EventLoopTimer*        EventLoopTimerRef;

/*
 *  EventLoopTimerProcPtr
 *  
 *  Discussion:
 *    Called when a timer fires.
 *  
 *  Parameters:
 *    
 *    inTimer:
 *      The timer that fired.
 *    
 *    inUserData:
 *      The data passed into InstallEventLoopTimer.
 */
typedef CALLBACK_API( void , EventLoopTimerProcPtr )(EventLoopTimerRef inTimer, void *inUserData);

/*
 *  Discussion:
 *    Event Loop Idle Timer Messages
 */
enum {

  /*
   * The user has gone idle (not touched an input device) for the
   * duration specified in your idle timer. This is the first message
   * you will receive. Start your engines!
   */
  kEventLoopIdleTimerStarted    = 1,

  /*
   * If you specified an interval on your idle timer, your idle timer
   * proc will be called with this message, letting you know it is
   * merely firing at the interval specified. You will receive this
   * message for the first time at the specified interval after you
   * receive kEventLoopIdleTimerStarted. If you did not specify an
   * interval, this message is not sent.
   */
  kEventLoopIdleTimerIdling     = 2,

  /*
   * The user is back! Stop everything! This is your cue to stop any
   * processing if you need to.
   */
  kEventLoopIdleTimerStopped    = 3
};

typedef UInt16                          EventLoopIdleTimerMessage;

/*
 *  EventLoopIdleTimerProcPtr
 *  
 *  Discussion:
 *    Called when an idle timer fires.
 *  
 *  Parameters:
 *    
 *    inTimer:
 *      The timer that fired.
 *    
 *    inState:
 *      The current state of the timer.
 *    
 *    inUserData:
 *      The data passed into InstallEventLoopTimer.
 */
typedef CALLBACK_API( void , EventLoopIdleTimerProcPtr )(EventLoopTimerRef inTimer, EventLoopIdleTimerMessage inState, void *inUserData);
typedef STACK_UPP_TYPE(EventLoopTimerProcPtr)                   EventLoopTimerUPP;
typedef STACK_UPP_TYPE(EventLoopIdleTimerProcPtr)               EventLoopIdleTimerUPP;
/*
 *  NewEventLoopTimerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern EventLoopTimerUPP
NewEventLoopTimerUPP(EventLoopTimerProcPtr userRoutine)       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewEventLoopIdleTimerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern EventLoopIdleTimerUPP
NewEventLoopIdleTimerUPP(EventLoopIdleTimerProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;

/*
 *  DisposeEventLoopTimerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeEventLoopTimerUPP(EventLoopTimerUPP userUPP)           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeEventLoopIdleTimerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeEventLoopIdleTimerUPP(EventLoopIdleTimerUPP userUPP)   AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;

/*
 *  InvokeEventLoopTimerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeEventLoopTimerUPP(
  EventLoopTimerRef  inTimer,
  void *             inUserData,
  EventLoopTimerUPP  userUPP)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeEventLoopIdleTimerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeEventLoopIdleTimerUPP(
  EventLoopTimerRef          inTimer,
  EventLoopIdleTimerMessage  inState,
  void *                     inUserData,
  EventLoopIdleTimerUPP      userUPP)                         AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;

/*
 *  InstallEventLoopTimer()
 *  
 *  Discussion:
 *    Installs a timer onto the event loop specified. The timer can
 *    either fire once or repeatedly at a specified interval depending
 *    on the parameters passed to this function.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEventLoop:
 *      The event loop to add the timer.
 *    
 *    inFireDelay:
 *      The delay before first firing this timer (can be 0, to request
 *      that the timer be fired as soon as control returns to your
 *      event loop). In Mac OS X and CarbonLib 1.5 and later, you may
 *      pass kEventDurationForever to stop the timer from firing at all
 *      until SetEventLoopTimerNextFireTime is used to start it; in
 *      earlier CarbonLibs, to achieve the same effect, just pass zero
 *      and then immediately call SetEventLoopTimerNextFireTime( timer,
 *      kEventDurationForever ) before returning control to your event
 *      loop.
 *    
 *    inInterval:
 *      The timer interval (pass 0 for a one-shot timer, which executes
 *      once but does not repeat). In Mac OS X and CarbonLib 1.5 and
 *      later, you may also pass kEventDurationForever to create a
 *      one-shot timer.
 *    
 *    inTimerProc:
 *      The routine to call when the timer fires.
 *    
 *    inTimerData:
 *      Data to pass to the timer proc when called.
 *    
 *    outTimer:
 *      A reference to the newly installed timer.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
InstallEventLoopTimer(
  EventLoopRef         inEventLoop,
  EventTimerInterval   inFireDelay,
  EventTimerInterval   inInterval,
  EventLoopTimerUPP    inTimerProc,
  void *               inTimerData,
  EventLoopTimerRef *  outTimer)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  InstallEventLoopIdleTimer()
 *  
 *  Discussion:
 *    Installs a timer onto the event loop specified. Idle timers are
 *    only called when there is no user activity occuring in the
 *    application. This means that the user is not actively
 *    clicking/typing, and is also not in the middle of tracking a
 *    control, menu, or window. TrackMouseLocation actually disables
 *    all idle timers automatically for you.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEventLoop:
 *      The event loop to add the timer.
 *    
 *    inDelay:
 *      The delay before firing this timer after a user input event has
 *      come in. For example, if you want to start your timer 2 seconds
 *      after the user stops typing, etc. you would pass 2.0 into this
 *      parameter. Each time the user types a key (or whatever), this
 *      timer is reset. If we are considered to be idle when an idle
 *      timer is installed, the first time it fires will be inDelay
 *      seconds from the time it is installed. So if you installed it
 *      in the middle of control tracking, say, it wouldn't fire until
 *      the user stopped tracking. But if you installed it at app
 *      startup and the user hasn't typed/clicked, it would fire in
 *      inDelay seconds.
 *    
 *    inInterval:
 *      The timer interval (pass 0 for a one-shot timer, which executes
 *      once but does not repeat). You may also pass
 *      kEventDurationForever to create a one-shot timer.
 *    
 *    inTimerProc:
 *      The routine to call when the timer fires.
 *    
 *    inTimerData:
 *      Data to pass to the timer proc when called.
 *    
 *    outTimer:
 *      A reference to the newly installed timer.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
InstallEventLoopIdleTimer(
  EventLoopRef            inEventLoop,
  EventTimerInterval      inDelay,
  EventTimerInterval      inInterval,
  EventLoopIdleTimerUPP   inTimerProc,
  void *                  inTimerData,
  EventLoopTimerRef *     outTimer)                           AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  RemoveEventLoopTimer()
 *  
 *  Discussion:
 *    Removes a timer that was previously installed by a call to
 *    InstallEventLoopTimer. You call this function when you are done
 *    using a timer.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inTimer:
 *      The timer to remove.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
RemoveEventLoopTimer(EventLoopTimerRef inTimer)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetEventLoopTimerNextFireTime()
 *  
 *  Discussion:
 *    This routine is used to 'reset' a timer. It controls the next
 *    time the timer fires. This will override any interval you might
 *    have set. For example, if you have a timer that fires every
 *    second, and you call this function setting the next time to five
 *    seconds from now, the timer will sleep for five seconds, then
 *    fire. It will then resume its one-second interval after that. It
 *    is as if you removed the timer and reinstalled it with a new
 *    first-fire delay.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inTimer:
 *      The timer to adjust
 *    
 *    inNextFire:
 *      The interval from the current time to wait until firing the
 *      timer again. You may pass kEventDurationForever to stop the
 *      timer from firing at all.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetEventLoopTimerNextFireTime(
  EventLoopTimerRef    inTimer,
  EventTimerInterval   inNextFire)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;




/*======================================================================================*/
/*  EVENT HANDLERS                                                                      */
/*======================================================================================*/

typedef struct OpaqueEventHandlerRef*   EventHandlerRef;
typedef struct OpaqueEventHandlerCallRef*  EventHandlerCallRef;

/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ EventHandler specification                                                        */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ C++ Methods as Event Handlers                                                     */
/*                                                                                      */
/* To use a C++ method as an Event Handler callback, it must be declared in its class   */
/* as a static method.  Otherwise, the implicit "this" parameter will make the function */
/* not match the EventHandlerProcPtr prototype.                                         */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/

/*
 *  EventHandlerProcPtr
 *  
 *  Discussion:
 *    Callback for receiving events sent to a target this callback is
 *    installed on.
 *  
 *  Parameters:
 *    
 *    inHandlerCallRef:
 *      A reference to the current handler call chain. This is sent to
 *      your handler so that you can call CallNextEventHandler if you
 *      need to.
 *    
 *    inEvent:
 *      The Event.
 *    
 *    inUserData:
 *      The app-specified data you passed in a call to
 *      InstallEventHandler.
 *  
 *  Result:
 *    An operating system result code. Returning noErr indicates you
 *    handled the event. Returning eventNotHandledErr indicates you did
 *    not handle the event and perhaps the toolbox should take other
 *    action.
 */
typedef CALLBACK_API( OSStatus , EventHandlerProcPtr )(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *inUserData);
typedef STACK_UPP_TYPE(EventHandlerProcPtr)                     EventHandlerUPP;
/*
 *  NewEventHandlerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern EventHandlerUPP
NewEventHandlerUPP(EventHandlerProcPtr userRoutine)           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeEventHandlerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeEventHandlerUPP(EventHandlerUPP userUPP)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeEventHandlerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSStatus
InvokeEventHandlerUPP(
  EventHandlerCallRef  inHandlerCallRef,
  EventRef             inEvent,
  void *               inUserData,
  EventHandlerUPP      userUPP)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

typedef struct OpaqueEventTargetRef*    EventTargetRef;
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ Installing Event Handlers                                                         */
/*                                                                                      */
/* Use these routines to install event handlers for a specific toolbox object. You may  */
/* pass zero for inNumTypes and NULL for inList if you need to be in a situation where  */
/* you know you will be receiving events, but not exactly which ones at the time you    */
/* are installing the handler. Later, your application can call the Add/Remove routines */
/* listed below this section.                                                           */
/*                                                                                      */
/* You can only install a specific handler once. The combination of inHandler and       */
/* inUserData is considered the 'signature' of a handler. Any attempt to install a new  */
/* handler with the same proc and user data as an already-installed handler will result */
/* in eventHandlerAlreadyInstalledErr. Installing the same proc and user data on a      */
/* different object is legal.                                                           */
/*                                                                                      */
/* Upon successful completion of this routine, you are returned an EventHandlerRef,     */
/* which you can use in various other calls, and is passed to your event handler. You   */
/* use it to extract information about the handler, such as the target (window, etc.)   */
/* if you have the same handler installed for different objects and need to perform     */
/* actions on the current target (say, call a window manager function).                 */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*
 *  InstallEventHandler()
 *  
 *  Discussion:
 *    Installs an event handler on a specified target. Your handler
 *    proc will be called with the events you registered with when an
 *    event of the corresponding type and class are send to the target
 *    you are installing your handler on.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inTarget:
 *      The target to register your handler with.
 *    
 *    inHandler:
 *      A pointer to your handler function.
 *    
 *    inNumTypes:
 *      The number of events you are registering for.
 *    
 *    inList:
 *      A pointer to an array of EventTypeSpec entries representing the
 *      events you are interested in.
 *    
 *    inUserData:
 *      The value passed in this parameter is passed on to your event
 *      handler proc when it is called.
 *    
 *    outRef:
 *      Receives an EventHandlerRef, which you can use later to remove
 *      the handler. You can pass null if you don't want the reference
 *      - when the target is disposed, the handler will be disposed as
 *      well.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
InstallEventHandler(
  EventTargetRef         inTarget,
  EventHandlerUPP        inHandler,
  UInt32                 inNumTypes,
  const EventTypeSpec *  inList,
  void *                 inUserData,
  EventHandlerRef *      outRef)           /* can be NULL */  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  InstallStandardEventHandler()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
InstallStandardEventHandler(EventTargetRef inTarget)          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  RemoveEventHandler()
 *  
 *  Discussion:
 *    Removes an event handler from the target it was bound to.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inHandlerRef:
 *      The handler ref to remove (returned in a call to
 *      InstallEventHandler). After you call this function, the handler
 *      ref is considered to be invalid and can no longer be used.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
RemoveEventHandler(EventHandlerRef inHandlerRef)              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ Adjusting set of event types after a handler is created                           */
/*                                                                                      */
/* After installing a handler with the routine above, you can adjust the event type     */
/* list telling the toolbox what events to send to that handler by calling the two      */
/* routines below. If you add an event type twice for the same handler, your handler    */
/* will only be called once, but it will take two RemoveEventType calls to stop your    */
/* handler from being called with that event type. In other words, the install count    */
/* for each event type is maintained by the toolbox. This might allow you, for example  */
/* to have subclasses of a window object register for types without caring if the base  */
/* class has already registered for that type. When the subclass removes its types, it  */
/* can successfully do so without affecting the base class's reception of its event     */
/* types, yielding eternal bliss.                                                       */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/

/*
 *  AddEventTypesToHandler()
 *  
 *  Discussion:
 *    Adds additional events to an event handler that has already been
 *    installed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inHandlerRef:
 *      The event handler to add the additional events to.
 *    
 *    inNumTypes:
 *      The number of events to add.
 *    
 *    inList:
 *      A pointer to an array of EventTypeSpec entries.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AddEventTypesToHandler(
  EventHandlerRef        inHandlerRef,
  UInt32                 inNumTypes,
  const EventTypeSpec *  inList)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  RemoveEventTypesFromHandler()
 *  
 *  Discussion:
 *    Removes events from an event handler that has already been
 *    installed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inHandlerRef:
 *      The event handler to remove the events from.
 *    
 *    inNumTypes:
 *      The number of events to remove.
 *    
 *    inList:
 *      A pointer to an array of EventTypeSpec entries.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
RemoveEventTypesFromHandler(
  EventHandlerRef        inHandlerRef,
  UInt32                 inNumTypes,
  const EventTypeSpec *  inList)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ Explicit Propagation                                                              */
/*                                                                                      */
/*  CallNextEventHandler can be used to call thru to all handlers below the current     */
/*  handler being called. You pass the EventHandlerCallRef passed to your EventHandler  */
/*  into this call so that we know how to properly forward the event. The result of     */
/*  this function should normally be the result of your own handler that you called     */
/*  this API from. The typical use of this routine would be to allow the toolbox to do  */
/*  its standard processing and then follow up with some type of embellishment.         */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/

/*
 *  CallNextEventHandler()
 *  
 *  Discussion:
 *    Calls thru to the event handlers below you in the event handler
 *    stack of the target to which your handler is bound. You might use
 *    this to call thru to the default toolbox handling in order to
 *    post-process the event. You can only call this routine from
 *    within an event handler.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inCallRef:
 *      The event handler call ref passed into your event handler.
 *    
 *    inEvent:
 *      The event to pass thru.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CallNextEventHandler(
  EventHandlerCallRef   inCallRef,
  EventRef              inEvent)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ Sending Events                                                                    */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/

/*
 *  Discussion:
 *    EventTarget Send Options
 */
enum {

  /*
   * The event should be sent to the target given only, and should not
   * propagate to any other target. CallNextEventHandler will do
   * nothing in a handler which has received an event sent in this
   * manner.
   */
  kEventTargetDontPropagate     = (1 << 0),

  /*
   * The event is a notification-style event, and should be received by
   * all handlers. The result is usually meaningless when sent in this
   * manner, though we do maintain the strongest result code while the
   * event falls through each handler. This means that if the first
   * handler to receive the event returned noErr, and the next returned
   * eventNotHandledErr, the result returned would actually be noErr.
   * No handler can stop this event from propagating, i.e. the result
   * code does not alter event flow.
   */
  kEventTargetSendToAllHandlers = (1 << 1)
};

/*
 *  SendEventToEventTarget()
 *  
 *  Discussion:
 *    Sends an event to the specified event target.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEvent:
 *      The event to send.
 *    
 *    inTarget:
 *      The target to send it to.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SendEventToEventTarget(
  EventRef         inEvent,
  EventTargetRef   inTarget)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SendEventToEventTargetWithOptions()
 *  
 *  Discussion:
 *    Sends an event to the specified event target, optionally
 *    controlling how the event propagates. See the discussion of the
 *    event send options above for more detail.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEvent:
 *      The event to send.
 *    
 *    inTarget:
 *      The target to send it to.
 *    
 *    inOptions:
 *      The options to modify the send behavior. Passing zero for this
 *      makes it behave just like SendEventToEventTarget.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SendEventToEventTargetWithOptions(
  EventRef         inEvent,
  EventTargetRef   inTarget,
  OptionBits       inOptions)                                 AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;



/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*  ₯ Secure Event Input                                                                */
/*ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ*/
/*
 *  EnableSecureEventInput()
 *  
 *  Summary:
 *    Enables secure event input mode.
 *  
 *  Discussion:
 *    When secure event input is enabled, keyboard input will only go
 *    to the application with keyboard focus, and will not be echoed to
 *    other applications that might be using the event monitor target
 *    to watch keyboard input. The EditText and EditUnicodeText
 *    controls automatically enter secure input mode when a password
 *    control has the focus; if your application implements its own
 *    password entry, you should enable secure event input while the
 *    user is entering text.
 *    
 *    This API maintains a count of the number of times that it has
 *    been called. Secure event input is not disabled until
 *    DisableSecureEventInput has been called the same number of
 *    times.
 *    
 *    Be sure to disable secure event input if your application becomes
 *    inactive. If your application crashes, secure event input will
 *    automatically be disabled if no other application has enabled it.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
EnableSecureEventInput(void)                                  AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  DisableSecureEventInput()
 *  
 *  Summary:
 *    Disables secure event input mode.
 *  
 *  Discussion:
 *    When secure event input is enabled, keyboard input will only go
 *    to the application with keyboard focus, and will not be echoed to
 *    other applications that might be using the event monitor target
 *    to watch keyboard input. The EditText and EditUnicodeText
 *    controls automatically enter secure input mode when a password
 *    control has the focus; if your application implements its own
 *    password entry, you should enable secure event input while the
 *    user is entering text.
 *    
 *    The EnableSecureEventInput API maintains a count of the number of
 *    times that it has been called. Secure event input is not disabled
 *    until this API has been called the same number of times.
 *    
 *    Be sure to disable secure event input if your application becomes
 *    inactive. If your application crashes, secure event input will
 *    automatically be disabled if no other application has enabled it.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
DisableSecureEventInput(void)                                 AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  IsSecureEventInputEnabled()
 *  
 *  Summary:
 *    Indicates whether secure event input is currently enabled.
 *  
 *  Discussion:
 *    This API returns whether secure event input is enabled by any
 *    process, not just the current process. Secure event input may be
 *    disabled in the current process but enabled in some other
 *    process; in that case, this API will return true.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
IsSecureEventInputEnabled(void)                               AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;



#pragma options align=reset

#ifdef __cplusplus
}
#endif

#endif /* __CARBONEVENTSCORE__ */

