//
// Copyright: Avnet, Softweb Inc. 2020
// Modified by Nik Markovic <nikola.markovic@avnet.com> on 6/24/21.
//

#ifndef IOTCONNECT_H
#define IOTCONNECT_H

#include <stddef.h>
#include <time.h>
#include "iotconnect_event.h"
#include "iotconnect_telemetry.h"
#include "iotconnect_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    // Authentication based on your CPID. Sync HTTP endpoint returns a long lived SAS token
    // This auth type is only intended as a simple way to connect your test and development devices
    // and must not be used in production
    IOTC_AT_TOKEN = 1,

    // CA Cert and Self Signed Cert
    IOTC_AT_X509 = 2,

    // TPM hardware devices
    IOTC_AT_TPM = 4, // 4 for compatibility with sync

    // IoTHub Key based authentication with Symmetric Keys (Primary or Secondary key)
    IOTC_AT_SYMMETRIC_KEY = 5

} IotConnectAuthType;

typedef enum {
    IOTC_CS_UNDEFINED,
    IOTC_CS_MQTT_CONNECTED,
    IOTC_CS_MQTT_DISCONNECTED
} IotConnectConnectionStatus;

typedef void (*IotConnectStatusCallback)(IotConnectConnectionStatus data);

typedef struct {
    IotConnectAuthType type;
    char* trust_store; // Path to a file containing the trust certificates for the remote MQTT host
    union {
        struct {
            char* device_cert; // Path to a file containing the device CA cert (or chain) in PEM format
            char* device_key; // Path to a file containing the device private key in PEM format
        } cert_info;
        char *symmetric_key;
        char *scope_id; // for TPM authentication. AKA: ID Scope
    } data;
} IotConnectAuthInfo;

typedef struct {
    char *env;    // Settings -> Key Vault -> CPID.
    char *cpid;   // Settings -> Key Vault -> Evnironment.
    char *duid;   // Name of the device.
    int qos; // QOS for outbound messages. Default 1.
    IotConnectAuthInfo auth_info;
    IotclOtaCallback ota_cb; // callback for OTA events.
    IotclCommandCallback cmd_cb; // callback for command events.
    IotclMessageCallback msg_cb; // callback for ALL messages, including the specific ones like cmd or ota callback.
    IotConnectStatusCallback status_cb; // callback for connection status
} IotConnectClientConfig;


IotConnectClientConfig *iotconnect_sdk_init_and_get_config();

// call iotconnect_sdk_init_and_get_config first and configure the SDK before calling iotconnect_sdk_init()
int iotconnect_sdk_init();

bool iotconnect_sdk_is_connected();

// Can be used to pass to telemetry functions
IotclConfig *iotconnect_sdk_get_lib_config();

// Run the MQTT loop. 
// This should be done periodically so that inbound events can be detect and pings processed.
// The function will block up to timeout_ms and issue callbacks for connection events or inbound messages if there are any.
void iotconnect_sdk_loop(unsigned int timeout_ms);

// blocks until sent and returns 0 if successful.
// data is a null-terminated string
int iotconnect_sdk_send_packet(const char *data);

void iotconnect_sdk_disconnect();

#ifdef __cplusplus
}
#endif

#endif