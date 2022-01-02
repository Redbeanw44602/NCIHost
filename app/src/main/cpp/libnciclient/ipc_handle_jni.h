/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler */

#ifndef _Included_cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler
#define _Included_cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler
 * Method:    ntInitForSocketDir
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler_ntInitForSocketDir
  (JNIEnv *, jclass, jstring);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler
 * Method:    ntPeekConnection
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler_ntPeekConnection
  (JNIEnv *, jclass);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler
 * Method:    ntRequestConnection
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler_ntRequestConnection
  (JNIEnv *, jclass);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler
 * Method:    ntWaitForConnection
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler_ntWaitForConnection
  (JNIEnv *, jclass, jint);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler
 * Method:    getIpcPidFileFD
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler_getIpcPidFileFD
  (JNIEnv *, jclass);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler
 * Method:    getKernelArchitecture
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler_getKernelArchitecture
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
/* Header for class cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler_IpcConnectionListener */

#ifndef _Included_cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler_IpcConnectionListener
#define _Included_cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler_IpcConnectionListener
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif
#endif
/* Header for class cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler_RemoteEventHandler */

#ifndef _Included_cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler_RemoteEventHandler
#define _Included_cc_ioctl_nfcdevicehost_ipc_daemon_IpcNativeHandler_RemoteEventHandler
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif
#endif
/* Header for class cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy */

#ifndef _Included_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
#define _Included_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    isConnected
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_isConnected
  (JNIEnv *, jobject);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    getVersionName
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_getVersionName
  (JNIEnv *, jobject);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    getVersionCode
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_getVersionCode
  (JNIEnv *, jobject);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    getBuildUuid
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_getBuildUuid
  (JNIEnv *, jobject);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    getSelfPid
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_getSelfPid
  (JNIEnv *, jobject);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    exitProcess
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_exitProcess
  (JNIEnv *, jobject);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    isDeviceSupported
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_isDeviceSupported
  (JNIEnv *, jobject);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    initHwServiceConnection
 * Signature: ([Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_initHwServiceConnection
  (JNIEnv *, jobject, jobjectArray);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    ntGetHistoryIoEvents
 * Signature: (II)Lcc/ioctl/nfcdevicehost/ipc/daemon/internal/NciHostDaemonProxy/RawHistoryIoEventList;
 */
JNIEXPORT jobject JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_ntGetHistoryIoEvents
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    deviceDriverWriteRaw
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_deviceDriverWriteRaw
  (JNIEnv *, jobject, jbyteArray);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    deviceDriverIoctl0
 * Signature: (IJ)I
 */
JNIEXPORT jint JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_deviceDriverIoctl0
  (JNIEnv *, jobject, jint, jlong);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    getDaemonStatus
 * Signature: ()Lcc/ioctl/nfcdevicehost/ipc/daemon/INciHostDaemon/DaemonStatus;
 */
JNIEXPORT jobject JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_getDaemonStatus
  (JNIEnv *, jobject);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    isAndroidNfcServiceConnected
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_isAndroidNfcServiceConnected
  (JNIEnv *, jobject);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    connectToAndroidNfcService
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_connectToAndroidNfcService
  (JNIEnv *, jobject);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    isNfcDiscoverySoundDisabled
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_isNfcDiscoverySoundDisabled
  (JNIEnv *, jobject);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    setNfcDiscoverySoundDisabled
 * Signature: (Z)Z
 */
JNIEXPORT jboolean JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_setNfcDiscoverySoundDisabled
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    clearHistoryIoEvents
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_clearHistoryIoEvents
  (JNIEnv *, jobject);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    getLogsPartial
 * Signature: (II)[Lcc/ioctl/nfcdevicehost/ipc/daemon/INciHostDaemon/LogEntryRecord;
 */
JNIEXPORT jobjectArray JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_getLogsPartial
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy
 * Method:    waitForEvent
 * Signature: ()Lcc/ioctl/nfcdevicehost/ipc/daemon/internal/NciHostDaemonProxy/NativeEventPacket;
 */
JNIEXPORT jobject JNICALL Java_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_waitForEvent
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
/* Header for class cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_RawHistoryIoEventList */

#ifndef _Included_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_RawHistoryIoEventList
#define _Included_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_RawHistoryIoEventList
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif
#endif
/* Header for class cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_RawIoEventPacket */

#ifndef _Included_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_RawIoEventPacket
#define _Included_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_RawIoEventPacket
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif
#endif
/* Header for class cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_NativeEventPacket */

#ifndef _Included_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_NativeEventPacket
#define _Included_cc_ioctl_nfcdevicehost_ipc_daemon_internal_NciHostDaemonProxy_NativeEventPacket
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif
#endif
