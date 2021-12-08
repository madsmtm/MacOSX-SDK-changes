/* iig(DriverKit-73.100.4) generated from IOUserNetworkPacketQueue.iig */

/* IOUserNetworkPacketQueue.iig:1-7 */
#ifndef _IOUSERNETWORKPACKETQUEUE_IIG
#define _IOUSERNETWORKPACKETQUEUE_IIG

#include <DriverKit/OSObject.h>  /* .iig include */
#include <NetworkingDriverKit/IOUserNetworkPacket.h>  /* .iig include */
#include <DriverKit/IODataQueueDispatchSource.h>  /* .iig include */

/* source class IOUserNetworkPacketQueue IOUserNetworkPacketQueue.iig:8-61 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

/*!
*/

class KERNEL IOUserNetworkPacketQueue : public OSObject
{
public:
    virtual bool
    init() override;

    virtual void
    free() override;

    virtual kern_return_t
    SetEnable(bool isEnable) = 0;

    virtual kern_return_t
    SetPacketBufferPool(
        IOUserNetworkPacketBufferPool * pool) LOCALONLY;

    virtual kern_return_t
    SetPacketDirection(
        IOUserNetworkPacketDirection    direction) LOCALONLY;

    virtual kern_return_t
    CopyDataQueue(
        IODataQueueDispatchSource ** dataQueue) LOCALONLY;

    virtual kern_return_t
    SetDataQueue(
        IODataQueueDispatchSource * dataQueue) LOCAL;

    virtual kern_return_t
    EnqueuePacket(
        IOUserNetworkPacket *    packet) LOCALONLY;

    virtual uint32_t
    EnqueuePackets(
        IOUserNetworkPacket **   packets,
        uint32_t                    packetCount) LOCALONLY;

    virtual kern_return_t
    DequeuePacket(
        IOUserNetworkPacket **   packet) LOCALONLY;

    virtual uint32_t
    DequeuePackets(
        IOUserNetworkPacket **   packets,
        uint32_t                    maxDequeueCount) LOCALONLY;
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IOUserNetworkPacketQueue IOUserNetworkPacketQueue.iig:8-61 */

#define IOUserNetworkPacketQueue_SetEnable_ID            0x8c7770e11b5d2d74ULL
#define IOUserNetworkPacketQueue_SetDataQueue_ID            0xc42e9c678d3ba3ebULL

#define IOUserNetworkPacketQueue_SetEnable_Args \
        bool isEnable

#define IOUserNetworkPacketQueue_SetDataQueue_Args \
        IODataQueueDispatchSource * dataQueue

#define IOUserNetworkPacketQueue_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IOUserNetworkPacketQueue * self, const IORPC rpc);\
\
    kern_return_t\
    SetEnable(\
        bool isEnable,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    SetDataQueue(\
        IODataQueueDispatchSource * dataQueue,\
        OSDispatchMethod supermethod = NULL);\
\
\
protected:\
    /* _Impl methods */\
\
    kern_return_t\
    SetDataQueue_Impl(IOUserNetworkPacketQueue_SetDataQueue_Args);\
\
\
public:\
    /* _Invoke methods */\
\
    typedef kern_return_t (*SetEnable_Handler)(OSMetaClassBase * target, IOUserNetworkPacketQueue_SetEnable_Args);\
    static kern_return_t\
    SetEnable_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetEnable_Handler func);\
\
    typedef kern_return_t (*SetDataQueue_Handler)(OSMetaClassBase * target, IOUserNetworkPacketQueue_SetDataQueue_Args);\
    static kern_return_t\
    SetDataQueue_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetDataQueue_Handler func);\
\


#define IOUserNetworkPacketQueue_KernelMethods \
\
protected:\
    /* _Impl methods */\
\


#define IOUserNetworkPacketQueue_VirtualMethods \
\
public:\
\
    virtual bool\
    init(\
) APPLE_KEXT_OVERRIDE;\
\
    virtual void\
    free(\
) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    SetPacketBufferPool(\
        IOUserNetworkPacketBufferPool * pool) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    SetPacketDirection(\
        IOUserNetworkPacketDirection direction) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    CopyDataQueue(\
        IODataQueueDispatchSource ** dataQueue) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    EnqueuePacket(\
        IOUserNetworkPacket * packet) APPLE_KEXT_OVERRIDE;\
\
    virtual uint32_t\
    EnqueuePackets(\
        IOUserNetworkPacket ** packets,\
        uint32_t packetCount) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    DequeuePacket(\
        IOUserNetworkPacket ** packet) APPLE_KEXT_OVERRIDE;\
\
    virtual uint32_t\
    DequeuePackets(\
        IOUserNetworkPacket ** packets,\
        uint32_t maxDequeueCount) APPLE_KEXT_OVERRIDE;\
\


#if !KERNEL

extern OSMetaClass          * gIOUserNetworkPacketQueueMetaClass;
extern const OSClassLoadInformation IOUserNetworkPacketQueue_Class;

class IOUserNetworkPacketQueueMetaClass : public OSMetaClass
{
public:
    virtual kern_return_t
    New(OSObject * instance) override;
    virtual kern_return_t
    Dispatch(const IORPC rpc) override;
};

#endif /* !KERNEL */

#if !KERNEL

class IOUserNetworkPacketQueueInterface : public OSInterface
{
public:
    virtual kern_return_t
    SetPacketBufferPool(IOUserNetworkPacketBufferPool * pool) = 0;

    virtual kern_return_t
    SetPacketDirection(IOUserNetworkPacketDirection direction) = 0;

    virtual kern_return_t
    CopyDataQueue(IODataQueueDispatchSource ** dataQueue) = 0;

    virtual kern_return_t
    EnqueuePacket(IOUserNetworkPacket * packet) = 0;

    virtual uint32_t
    EnqueuePackets(IOUserNetworkPacket ** packets,
        uint32_t packetCount) = 0;

    virtual kern_return_t
    DequeuePacket(IOUserNetworkPacket ** packet) = 0;

    virtual uint32_t
    DequeuePackets(IOUserNetworkPacket ** packets,
        uint32_t maxDequeueCount) = 0;

};

struct IOUserNetworkPacketQueue_IVars;
struct IOUserNetworkPacketQueue_LocalIVars;

class IOUserNetworkPacketQueue : public OSObject, public IOUserNetworkPacketQueueInterface
{
#if !KERNEL
    friend class IOUserNetworkPacketQueueMetaClass;
#endif /* !KERNEL */

#if !KERNEL
public:
    union
    {
        IOUserNetworkPacketQueue_IVars * ivars;
        IOUserNetworkPacketQueue_LocalIVars * lvars;
    };
#endif /* !KERNEL */

    using super = OSObject;

#if !KERNEL
    IOUserNetworkPacketQueue_Methods
    IOUserNetworkPacketQueue_VirtualMethods
#endif /* !KERNEL */

};
#endif /* !KERNEL */


#endif /* !__DOCUMENTATION__ */

/* IOUserNetworkPacketQueue.iig:63- */

#endif /* ! _IOUSERNETWORKPACKETQUEUE_IIG */
