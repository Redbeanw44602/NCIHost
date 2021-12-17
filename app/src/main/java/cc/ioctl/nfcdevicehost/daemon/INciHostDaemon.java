package cc.ioctl.nfcdevicehost.daemon;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.io.IOException;

import cc.ioctl.nfcdevicehost.daemon.internal.NciHostDaemonProxy;
import cc.ioctl.nfcdevicehost.util.ByteUtils;

public interface INciHostDaemon {
    boolean isConnected();

    String getVersionName();

    int getVersionCode();

    String getBuildUuid();

    int getSelfPid();

    void exitProcess();

    boolean isDeviceSupported();

    boolean isHwServiceConnected();

    HistoryIoEventList getHistoryIoEvents(int startIndex, int count);

    boolean clearHistoryIoEvents();

    boolean initHwServiceConnection(String soPath) throws IOException;

    int deviceDriverWriteRaw(@NonNull byte[] data);

    int deviceDriverIoctl0(long request, long arg);

    interface OnRemoteEventListener {
        void onIoEvent(IoEventPacket event);

        void onRemoteDeath(RemoteDeathPacket event);
    }

    void registerRemoteEventListener(@NonNull OnRemoteEventListener listener);

    boolean unregisterRemoteEventListener(@NonNull OnRemoteEventListener listener);

    class DaemonStatus {
        public int processId;
        @NonNull
        public String versionName;
        public int abiArch;
        @Nullable
        public String daemonProcessSecurityContext;
        public boolean isHalServiceAttached;
        public int halServicePid;
        public int halServiceUid;
        @Nullable
        public String halServiceExePath;
        public int halServiceArch;
        @Nullable
        public String halServiceProcessSecurityContext;
        @Nullable
        public String halServiceExecutableSecurityLabel;

        public DaemonStatus(int processId, @NonNull String versionName, int abiArch, @Nullable String daemonProcessSecurityContext,
                            boolean isHalServiceAttached, int halServicePid, int halServiceUid,
                            @Nullable String halServiceExePath, int halServiceArch,
                            @Nullable String halServiceProcessSecurityContext, @Nullable String halServiceExecutableSecurityLabel) {
            this.processId = processId;
            this.versionName = versionName;
            this.abiArch = abiArch;
            this.daemonProcessSecurityContext = daemonProcessSecurityContext;
            this.isHalServiceAttached = isHalServiceAttached;
            this.halServicePid = halServicePid;
            this.halServiceUid = halServiceUid;
            this.halServiceExePath = halServiceExePath;
            this.halServiceArch = halServiceArch;
            this.halServiceProcessSecurityContext = halServiceProcessSecurityContext;
            this.halServiceExecutableSecurityLabel = halServiceExecutableSecurityLabel;
        }

        @Override
        public String toString() {
            return "DaemonStatus{" +
                    "processId=" + processId +
                    ", versionName='" + versionName + '\'' +
                    ", abiArch=" + abiArch +
                    ", daemonProcessSecurityContext='" + daemonProcessSecurityContext + '\'' +
                    ", isHalServiceAttached=" + isHalServiceAttached +
                    ", halServicePid=" + halServicePid +
                    ", halServiceUid=" + halServiceUid +
                    ", halServiceExePath='" + halServiceExePath + '\'' +
                    ", halServiceArch=" + halServiceArch +
                    ", halServiceProcessSecurityContext='" + halServiceProcessSecurityContext + '\'' +
                    ", halServiceExecutableSecurityLabel='" + halServiceExecutableSecurityLabel + '\'' +
                    '}';
        }
    }

    @NonNull
    DaemonStatus getDaemonStatus();

    class IoEventPacket implements NciHostDaemonProxy.NativeEventPacket {
        public enum IoOperationType {
            OPEN(1),
            CLOSE(2),
            READ(3),
            WRITE(4),
            IOCTL(5),
            SELECT(6);

            final int value;

            IoOperationType(int v) {
                value = v;
            }

            public int getValue() {
                return value;
            }

            public static IoOperationType fromValue(int v) {
                switch (v) {
                    case 1:
                        return OPEN;
                    case 2:
                        return CLOSE;
                    case 3:
                        return READ;
                    case 4:
                        return WRITE;
                    case 5:
                        return IOCTL;
                    case 6:
                        return SELECT;
                    default:
                        throw new IllegalArgumentException("Invalid value: " + v);
                }
            }
        }

        public int sequence;
        public long timestamp;
        public int fd;
        public IoOperationType opType;
        public long retValue;
        public long directArg1;
        public long directArg2;
        public byte[] buffer;

        public IoEventPacket() {
        }

        //struct IoOperationEvent {
        //    uint32_t sequence;
        //    uint32_t rfu;
        //    uint64_t timestamp;
        //    IoSyscallInfo info;
        //};
        //struct IoSyscallInfo {
        //    int32_t opType;
        //    int32_t fd;
        //    int64_t retValue;
        //    uint64_t directArg1;
        //    uint64_t directArg2;
        //    int64_t bufferLength;
        //};
        //

        public IoEventPacket(NciHostDaemonProxy.RawIoEventPacket raw) {
            sequence = ByteUtils.readInt32(raw.event, 0);
            // rfu ignored
            timestamp = ByteUtils.readInt64(raw.event, 8);
            int op = ByteUtils.readInt32(raw.event, 16);
            opType = IoOperationType.fromValue(op);
            fd = ByteUtils.readInt32(raw.event, 20);
            retValue = ByteUtils.readInt64(raw.event, 24);
            directArg1 = ByteUtils.readInt64(raw.event, 32);
            directArg2 = ByteUtils.readInt64(raw.event, 40);
            int bufferLength = ByteUtils.readInt32(raw.event, 48);
            buffer = raw.payload;
            if (bufferLength != (buffer == null ? 0 : buffer.length)) {
                throw new IllegalArgumentException("buffer length mismatch");
            }
        }
    }

    class RemoteDeathPacket implements NciHostDaemonProxy.NativeEventPacket {
        public int pid;

        public RemoteDeathPacket() {
        }

        public RemoteDeathPacket(int pid) {
            this.pid = pid;
        }
    }

    class HistoryIoEventList {
        public int totalStartIndex;
        public int totalCount;
        public IoEventPacket[] events;
    }
}
