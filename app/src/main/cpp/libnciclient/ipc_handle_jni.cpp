//
// Created by kinit on 2021-05-15.
//

#include <sys/un.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <jni.h>
#include <cerrno>
#include <mutex>
#include <vector>
#include <tuple>
#include <condition_variable>

#include "ipc_handle_jni.h"
#include "IpcConnector.h"
#include "../rpcprotocol/log/Log.h"
#include "NciClientImpl.h"

#define LOG_TAG "ipc_handle_jni"

using namespace std;
using namespace ipcprotocol;

static std::mutex g_EventMutex;
static std::condition_variable g_EventWaitCondition;
static std::vector<std::tuple<halpatch::IoOperationEvent, std::vector<uint8_t>>> g_IoEventVec;
static std::vector<int> g_RemoteDeathVec;

extern "C" void __android_log_print(int level, const char *tag, const char *fmt, ...);

static void android_log_handler(Log::Level level, const char *tag, const char *msg) {
    __android_log_print(static_cast<int>(level), tag, "%s", msg);
}

template<class T>
bool jniThrowLpcResultErrorOrException(JNIEnv *env, const TypedLpcResult<T> &result) {
    if (uint32_t err = result.getError(); err != 0) {
        std::string msg;
        switch (LpcErrorCode(err)) {
            case LpcErrorCode::ERR_NO_LPC_HANDLER:
                msg = "no LPC support in the remote end";
                break;
            case LpcErrorCode::ERR_BROKEN_CONN:
                msg = "connection broken";
                break;
            case LpcErrorCode::ERR_INVALID_ARGUMENT:
                msg = "argument list mismatch";
                break;
            case LpcErrorCode::ERR_LOCAL_INTERNAL_ERROR:
                msg = "ERR_LOCAL_INTERNAL_ERROR";
                break;
            case LpcErrorCode::ERR_BAD_REQUEST:
                msg = "ERR_BAD_BUFFER";
                break;
            case LpcErrorCode::ERR_REMOTE_INTERNAL_ERROR:
                msg = "ERR_REMOTE_INTERNAL_ERROR";
                break;
            case LpcErrorCode::ERR_TIMEOUT_IN_CRITICAL_CONTEXT:
                msg = "ERR_TIMEOUT_IN_CRITICAL_CONTEXT";
                break;
            default:
                msg = std::string("unknown error: ") + std::to_string(err);
        }
        env->ThrowNew(env->FindClass("android/os/RemoteException"), msg.c_str());
        return true;
    } else if (result.hasException()) {
        if (RemoteException ex; !result.getException(&ex)) {
            env->ThrowNew(env->FindClass("java/lang/RuntimeException"),
                          "error while read exception from LpcResult");
        } else {
            env->ThrowNew(env->FindClass("java/lang/RuntimeException"), ex.what());
        }
        return true;
    }
    return false;
}

template<class T>
bool jniThrowLpcResultErrorOrCorruption(JNIEnv *env, const TypedLpcResult<T> &result) {
    if (uint32_t err = result.getError(); err != 0) {
        std::string msg;
        switch (LpcErrorCode(err)) {
            case LpcErrorCode::ERR_NO_LPC_HANDLER:
                msg = "no LPC support in the remote end";
                break;
            case LpcErrorCode::ERR_BROKEN_CONN:
                msg = "connection broken";
                break;
            case LpcErrorCode::ERR_INVALID_ARGUMENT:
                msg = "argument list mismatch";
                break;
            case LpcErrorCode::ERR_LOCAL_INTERNAL_ERROR:
                msg = "ERR_LOCAL_INTERNAL_ERROR";
                break;
            case LpcErrorCode::ERR_BAD_REQUEST:
                msg = "ERR_BAD_BUFFER";
                break;
            case LpcErrorCode::ERR_REMOTE_INTERNAL_ERROR:
                msg = "ERR_REMOTE_INTERNAL_ERROR";
                break;
            case LpcErrorCode::ERR_TIMEOUT_IN_CRITICAL_CONTEXT:
                msg = "ERR_TIMEOUT_IN_CRITICAL_CONTEXT";
                break;
            default:
                msg = std::string("unknown error: ") + std::to_string(err);
        }
        env->ThrowNew(env->FindClass("android/os/RemoteException"), msg.c_str());
        return true;
    } else if (result.hasException()) {
        if (RemoteException ex; !result.getException(&ex)) {
            env->ThrowNew(env->FindClass("java/lang/RuntimeException"),
                          "error while read exception from LpcResult");
            return true;
        }
        return false;
    }
    return false;
}

extern "C" [[maybe_unused]] JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    Log::setLogHandler(&android_log_handler);
    (void) IpcConnector::getInstance();
    return JNI_VERSION_1_6;
}

extern "C" [[maybe_unused]] JNIEXPORT void JNICALL
Java_cc_ioctl_nfcncihost_daemon_IpcNativeHandler_ntInitForSocketDir
        (JNIEnv *env, jclass clazz, jstring name) {
    char buf[256];
    int len = env->GetStringLength(name);
    if (len > 108) {
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"), "path too long");
        return;
    }
    const char *dirPath = env->GetStringUTFChars(name, nullptr);
    if (dirPath == nullptr) {
        return;// with exception
    }
    jclass cUtils = env->FindClass("cc/ioctl/nfcncihost/util/Utils");
    jstring uuid = (jstring) env->
            CallStaticObjectMethod(cUtils, env->
            GetStaticMethodID(cUtils, "generateUuid", "()Ljava/lang/String;"));
    char uuidStr[64] = {};
    env->GetStringUTFRegion(uuid, 0, env->GetStringLength(uuid), uuidStr);
    IpcConnector &connector = IpcConnector::getInstance();
    int err = connector.init(dirPath, uuidStr);
    env->ReleaseStringUTFChars(name, dirPath);
    if (err != 0) {
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"),
                      (std::string("error init ipc handle: errno: ")
                       + std::to_string(err) + ", " + strerror(err)).c_str());
    }
}


extern "C" [[maybe_unused]] JNIEXPORT jboolean JNICALL
Java_cc_ioctl_nfcncihost_daemon_IpcNativeHandler_ntPeekConnection
        (JNIEnv *env, jclass clazz) {
    IpcConnector &connector = IpcConnector::getInstance();
    return connector.isConnected();
}

extern "C" [[maybe_unused]] JNIEXPORT jboolean JNICALL
Java_cc_ioctl_nfcncihost_daemon_IpcNativeHandler_ntRequestConnection
        (JNIEnv *env, jclass) {
    IpcConnector &connector = IpcConnector::getInstance();
    if (connector.isInitialized()) {
        if (connector.isConnected()) {
            return true;
        } else {
            int errcode = connector.sendConnectRequest();
            if (errcode != 0) {
                env->ThrowNew(env->FindClass("java/lang/RuntimeException"),
                              strerror(errcode));
                return false;
            } else {
                return false;
            }
        }
    } else {
        env->ThrowNew(env->FindClass("java/lang/IllegalStateException"),
                      "attempt to connect before initialization");
        return false;
    }
}

extern "C" [[maybe_unused]] JNIEXPORT jboolean JNICALL
Java_cc_ioctl_nfcncihost_daemon_IpcNativeHandler_ntWaitForConnection
        (JNIEnv *env, jclass, jint timeout) {
    IpcConnector &connector = IpcConnector::getInstance();
    if (connector.isInitialized()) {
        return (jboolean) (connector.waitForConnection(timeout >= -1 ? timeout : 0) > 0);
    } else {
        env->ThrowNew(env->FindClass("java/lang/IllegalStateException"),
                      "attempt to connect before initialization");
        return false;
    }
}
/*
 * Class:     cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy
 * Method:    isConnected
 * Signature: ()Z
 */
extern "C" [[maybe_unused]] JNIEXPORT jboolean JNICALL
Java_cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy_isConnected
        (JNIEnv *, jobject) {
    IpcConnector &connector = IpcConnector::getInstance();
    return connector.isConnected();
}

/*
 * Class:     cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy
 * Method:    getVersionName
 * Signature: ()Ljava/lang/String;
 */
extern "C" [[maybe_unused]] JNIEXPORT jstring JNICALL
Java_cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy_getVersionName
        (JNIEnv *env, jobject) {
    IpcConnector &connector = IpcConnector::getInstance();
    INciHostDaemon *proxy = connector.getNciDaemon();
    if (proxy == nullptr) {
        env->ThrowNew(env->FindClass("java/lang/IllegalStateException"),
                      "attempt to transact while proxy object not available");
        return nullptr;
    } else {
        if (auto lpcResult = proxy->getVersionName();
                !jniThrowLpcResultErrorOrException(env, lpcResult)) {
            std::string r;
            if (lpcResult.getResult(&r)) {
                return env->NewStringUTF(r.c_str());
            } else {
                env->ThrowNew(env->FindClass("java/lang/RuntimeException"),
                              "error while read data from LpcResult");
                return nullptr;
            }
        }
        return nullptr;
    }
}

/*
 * Class:     cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy
 * Method:    getVersionCode
 * Signature: ()I
 */
extern "C" [[maybe_unused]] JNIEXPORT jint JNICALL
Java_cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy_getVersionCode
        (JNIEnv *env, jobject) {
    IpcConnector &connector = IpcConnector::getInstance();
    INciHostDaemon *proxy = connector.getNciDaemon();
    if (proxy == nullptr) {
        env->ThrowNew(env->FindClass("java/lang/IllegalStateException"),
                      "attempt to transact while proxy object not available");
        return 0;
    } else {
        if (auto lpcResult = proxy->getVersionCode();
                !jniThrowLpcResultErrorOrException(env, lpcResult)) {
            int r;
            if (lpcResult.getResult(&r)) {
                return r;
            } else {
                env->ThrowNew(env->FindClass("java/lang/RuntimeException"),
                              "error while read data from LpcResult");
                return 0;
            }
        }
        return 0;
    }
}

/*
 * Class:     cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy
 * Method:    getBuildUuid
 * Signature: ()Ljava/lang/String;
 */
extern "C" [[maybe_unused]] JNIEXPORT jstring JNICALL
Java_cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy_getBuildUuid
        (JNIEnv *env, jobject) {
    IpcConnector &connector = IpcConnector::getInstance();
    INciHostDaemon *proxy = connector.getNciDaemon();
    if (proxy == nullptr) {
        env->ThrowNew(env->FindClass("java/lang/IllegalStateException"),
                      "attempt to transact while proxy object not available");
        return nullptr;
    } else {
        if (auto lpcResult = proxy->getBuildUuid();
                !jniThrowLpcResultErrorOrException(env, lpcResult)) {
            std::string r;
            if (lpcResult.getResult(&r)) {
                return env->NewStringUTF(r.c_str());
            } else {
                env->ThrowNew(env->FindClass("java/lang/RuntimeException"),
                              "error while read data from LpcResult");
                return nullptr;
            }
        }
        return nullptr;
    }
}

/*
 * Class:     cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy
 * Method:    exitProcess
 * Signature: ()V
 */
extern "C" [[maybe_unused]] JNIEXPORT void JNICALL
Java_cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy_exitProcess
        (JNIEnv *env, jobject) {
    IpcConnector &connector = IpcConnector::getInstance();
    INciHostDaemon *proxy = connector.getNciDaemon();
    if (proxy == nullptr) {
        env->ThrowNew(env->FindClass("java/lang/IllegalStateException"),
                      "attempt to transact while proxy object not available");
    } else {
        if (auto lpcResult = proxy->exitProcess();
                !jniThrowLpcResultErrorOrException(env, lpcResult)) {
            if (lpcResult.isTransacted()) {
                return;
            } else {
                env->ThrowNew(env->FindClass("java/lang/RuntimeException"),
                              "error while read data from LpcResult");
            }
        }
    }
}

/*
 * Class:     cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy
 * Method:    isDeviceSupported
 * Signature: ()Z
 */
extern "C" [[maybe_unused]] JNIEXPORT jboolean JNICALL
Java_cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy_isDeviceSupported
        (JNIEnv *env, jobject) {
    IpcConnector &connector = IpcConnector::getInstance();
    INciHostDaemon *proxy = connector.getNciDaemon();
    if (proxy == nullptr) {
        env->ThrowNew(env->FindClass("java/lang/IllegalStateException"),
                      "attempt to transact while proxy object not available");
        return 0;
    } else {
        if (auto lpcResult = proxy->isDeviceSupported();
                !jniThrowLpcResultErrorOrException(env, lpcResult)) {
            bool r;
            if (lpcResult.getResult(&r)) {
                return r;
            } else {
                env->ThrowNew(env->FindClass("java/lang/RuntimeException"),
                              "error while read data from LpcResult");
                return 0;
            }
        }
        return 0;
    }
}

/*
 * Class:     cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy
 * Method:    isHwServiceConnected
 * Signature: ()Z
 */
extern "C" [[maybe_unused]] JNIEXPORT jboolean JNICALL
Java_cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy_isHwServiceConnected
        (JNIEnv *env, jobject) {
    IpcConnector &connector = IpcConnector::getInstance();
    INciHostDaemon *proxy = connector.getNciDaemon();
    if (proxy == nullptr) {
        env->ThrowNew(env->FindClass("java/lang/IllegalStateException"),
                      "attempt to transact while proxy object not available");
        return 0;
    } else {
        if (auto lpcResult = proxy->isHwServiceConnected();
                !jniThrowLpcResultErrorOrException(env, lpcResult)) {
            bool r;
            if (lpcResult.getResult(&r)) {
                return r;
            } else {
                env->ThrowNew(env->FindClass("java/lang/RuntimeException"),
                              "error while read data from LpcResult");
                return 0;
            }
        }
        return 0;
    }
}

/*
 * Class:     cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy
 * Method:    initHwServiceConnection
 * Signature: (Ljava/lang/String;)Z
 */
extern "C" [[maybe_unused]] JNIEXPORT jboolean JNICALL
Java_cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy_initHwServiceConnection
        (JNIEnv *env, jobject, jstring jstrSoPath) {
    if (jstrSoPath == nullptr) {
        env->ThrowNew(env->FindClass("java/lang/NullPointerException"),
                      "null so path");
        return 0;
    }
    const char *strSoPath = env->GetStringUTFChars(jstrSoPath, nullptr);
    if (strSoPath == nullptr) {
        env->ThrowNew(env->FindClass("java/lang/OutOfMemoryError"),
                      "out of memory");
        return 0;
    }
    std::string soPath(strSoPath);
    env->ReleaseStringUTFChars(jstrSoPath, strSoPath);
    IpcConnector &connector = IpcConnector::getInstance();
    INciHostDaemon *proxy = connector.getNciDaemon();
    if (proxy == nullptr) {
        env->ThrowNew(env->FindClass("java/lang/IllegalStateException"),
                      "attempt to transact while proxy object not available");
        return false;
    } else {
        if (auto lpcResult = proxy->initHwServiceConnection(soPath);
                !jniThrowLpcResultErrorOrCorruption(env, lpcResult)) {
            bool r;
            if (lpcResult.hasException()) {
                RemoteException exception = {1, EBADMSG, "invalid exception data structure"};
                (void) lpcResult.getException(&exception);
                env->ThrowNew(env->FindClass("java/io/IOException"), exception.what());
                return false;
            }
            if (lpcResult.getResult(&r)) {
                return r;
            } else {
                env->ThrowNew(env->FindClass("java/lang/RuntimeException"),
                              "error while read data from LpcResult");
                return false;
            }
        }
        return false;
    }
}


void NciClientImpl_forwardRemoteIoEvent(const halpatch::IoOperationEvent &event, const std::vector<uint8_t> &payload) {
    std::scoped_lock<std::mutex> lock(g_EventMutex);
    g_IoEventVec.emplace_back(event, payload);
    g_EventWaitCondition.notify_all();
}

void NciClientImpl_forwardRemoteDeathEvent(int pid) {
    std::scoped_lock<std::mutex> lock(g_EventMutex);
    g_RemoteDeathVec.emplace_back(pid);
    g_EventWaitCondition.notify_all();
}

/*
 * Class:     cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy
 * Method:    waitForEvent
 * Signature: ()Lcc/ioctl/nfcncihost/daemon/internal/NciHostDaemonProxy/NativeEventPacket;
 */
extern "C" [[maybe_unused]] JNIEXPORT jobject JNICALL
Java_cc_ioctl_nfcncihost_daemon_internal_NciHostDaemonProxy_waitForEvent
        (JNIEnv *env, jobject) {
    std::unique_lock<std::mutex> lock(g_EventMutex);
    while (g_IoEventVec.empty() && g_RemoteDeathVec.empty()) {
        g_EventWaitCondition.wait(lock);
    }
    // check for remote death
    if (!g_RemoteDeathVec.empty()) {
        int deathEvent = g_RemoteDeathVec.back();
        g_RemoteDeathVec.pop_back();
        jmethodID ctor = env->GetMethodID(
                env->FindClass("cc/ioctl/nfcncihost/daemon/internal/NciHostDaemonProxy$RemoteDeathPacket"),
                "<init>", "(I)V");
        jobject packet = env->NewObject(
                env->FindClass("cc/ioctl/nfcncihost/daemon/internal/NciHostDaemonProxy$RemoteDeathPacket"),
                ctor, deathEvent);
        return packet;
    }
    // check for io event
    if (!g_IoEventVec.empty()) {
        halpatch::IoOperationEvent event = std::get<0>(g_IoEventVec.back());
        std::vector<uint8_t> payload = std::get<1>(g_IoEventVec.back());
        g_IoEventVec.pop_back();
        jmethodID ctor = env->GetMethodID(
                env->FindClass("cc/ioctl/nfcncihost/daemon/internal/NciHostDaemonProxy$RawIoEventPacket"),
                "<init>", "([B[B)V");
        jbyteArray buffer1 = env->NewByteArray(sizeof(halpatch::IoOperationEvent));
        env->SetByteArrayRegion(buffer1, 0, sizeof(halpatch::IoOperationEvent), (jbyte *) &event);
        jbyteArray buffer2 = nullptr;
        if (payload.size() > 0) {
            buffer2 = env->NewByteArray(payload.size());
            env->SetByteArrayRegion(buffer2, 0, payload.size(), (jbyte *) payload.data());
        }
        jobject packet = env->NewObject(
                env->FindClass("cc/ioctl/nfcncihost/daemon/internal/NciHostDaemonProxy$RawIoEventPacket"),
                ctor, buffer1, buffer2);
        return packet;
    }
    // should not happen
    env->ThrowNew(env->FindClass("java/io/IOException"), "internal error");
    return nullptr;
}

/*
 * Class:     cc_ioctl_nfcncihost_daemon_IpcNativeHandler
 * Method:    getKernelArchitecture
 * Signature: ()Ljava/lang/String;
 */
extern "C" [[maybe_unused]] JNIEXPORT jstring JNICALL
Java_cc_ioctl_nfcncihost_daemon_IpcNativeHandler_getKernelArchitecture
        (JNIEnv *env, jclass) {
    struct utsname uts = {};
    if (uname(&uts) != 0) {
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"),
                      ("uname() failed: " + std::string(strerror(errno))).c_str());
        return nullptr;
    }
    return env->NewStringUTF(uts.machine);
}

/*
 * Class:     cc_ioctl_nfcncihost_daemon_IpcNativeHandler
 * Method:    getIpcPidFileFD
 * Signature: ()I
 */
extern "C" [[maybe_unused]] JNIEXPORT jint JNICALL
Java_cc_ioctl_nfcncihost_daemon_IpcNativeHandler_getIpcPidFileFD
        (JNIEnv *, jclass) {
    const IpcConnector &connector = IpcConnector::getInstance();
    return int(connector.getIpcFileFlagFd());
}
