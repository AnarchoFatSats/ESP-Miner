#ifndef GLOBAL_STATE_H_
#define GLOBAL_STATE_H_

#include <stdbool.h>
#include <stdint.h>
#include "asic_task.h"
#include "common.h"
#include "power_management_task.h"
#include "serial.h"
#include "stratum_api.h"
#include "work_queue.h"

#define STRATUM_USER CONFIG_STRATUM_USER
#define FALLBACK_STRATUM_USER CONFIG_FALLBACK_STRATUM_USER

#define HISTORY_LENGTH 100
#define DIFF_STRING_SIZE 10

// Thermal management constants
#define DEFAULT_THERMAL_THROTTLE_TEMP 75.0f
#define DEFAULT_THERMAL_SHUTDOWN_TEMP 85.0f

// Power management constants
#define POWER_EFFICIENCY_THRESHOLD 0.9f
#define MIN_FREQUENCY_FACTOR 0.75f

typedef enum
{
    DEVICE_UNKNOWN = -1,
    DEVICE_MAX,
    DEVICE_ULTRA,
    DEVICE_SUPRA,
    DEVICE_GAMMA,
    DEVICE_GAMMATURBO,
} DeviceModel;

typedef enum
{
    ASIC_UNKNOWN = -1,
    ASIC_BM1397,
    ASIC_BM1366,
    ASIC_BM1368,
    ASIC_BM1370,
} AsicModel;

// Power management profiles
typedef enum {
    POWER_PROFILE_PERFORMANCE,   // Maximum performance, highest power usage
    POWER_PROFILE_BALANCED,      // Balanced between performance and efficiency
    POWER_PROFILE_EFFICIENCY,    // Optimized for power efficiency
    POWER_PROFILE_DYNAMIC        // Dynamically adjusted based on conditions
} PowerProfile;

// typedef struct
// {
//     uint8_t (*init_fn)(uint64_t, uint16_t);
//     task_result * (*receive_result_fn)(void * GLOBAL_STATE);
//     int (*set_max_baud_fn)(void);
//     void (*set_difficulty_mask_fn)(int);
//     void (*send_work_fn)(void * GLOBAL_STATE, bm_job * next_bm_job);
//     void (*set_version_mask)(uint32_t);
// } AsicFunctions;

typedef struct {
    char message[64];
    uint32_t count;
} RejectedReasonStat;

typedef struct
{
    double duration_start;
    int historical_hashrate_rolling_index;
    double historical_hashrate_time_stamps[HISTORY_LENGTH];
    double historical_hashrate[HISTORY_LENGTH];
    int historical_hashrate_init;
    double current_hashrate;
    int64_t start_time;
    uint64_t shares_accepted;
    uint64_t shares_rejected;
    RejectedReasonStat rejected_reason_stats[10];
    int rejected_reason_stats_count;
    int screen_page;
    uint64_t best_nonce_diff;
    char best_diff_string[DIFF_STRING_SIZE];
    uint64_t best_session_nonce_diff;
    char best_session_diff_string[DIFF_STRING_SIZE];
    bool FOUND_BLOCK;
    char ssid[32];
    char wifi_status[256];
    char ip_addr_str[16]; // IP4ADDR_STRLEN_MAX
    char ap_ssid[32];
    bool ap_enabled;
    bool is_connected;
    char * pool_url;
    char * fallback_pool_url;
    uint16_t pool_port;
    uint16_t fallback_pool_port;
    char * pool_user;
    char * fallback_pool_user;
    char * pool_pass;
    char * fallback_pool_pass;
    bool is_using_fallback;
    uint16_t overheat_mode;
    uint16_t power_fault;
    uint32_t lastClockSync;
    bool is_screen_active;
    bool is_firmware_update;
    char firmware_update_filename[20];
    char firmware_update_status[20];
    char * asic_status;
    float thermal_throttle_temp;
    float thermal_shutdown_temp;
    PowerProfile power_profile;
} SystemModule;

typedef struct
{
    bool active;
    char *message;
    bool result;
    bool finished;
} SelfTestModule;

typedef struct
{
    DeviceModel device_model;
    char * device_model_str;
    int board_version;
    AsicModel asic_model;
    char * asic_model_str;
    double asic_job_frequency_ms;
    uint32_t ASIC_difficulty;

    work_queue stratum_queue;
    work_queue ASIC_jobs_queue;

    SystemModule SYSTEM_MODULE;
    AsicTaskModule ASIC_TASK_MODULE;
    PowerManagementModule POWER_MANAGEMENT_MODULE;
    SelfTestModule SELF_TEST_MODULE;

    char * extranonce_str;
    int extranonce_2_len;
    int abandon_work;

    uint8_t * valid_jobs;
    pthread_mutex_t valid_jobs_lock;

    uint32_t stratum_difficulty;
    uint32_t version_mask;
    bool new_stratum_version_rolling_msg;

    int sock;

    // A message ID that must be unique per request that expects a response.
    // For requests not expecting a response (called notifications), this is null.
    int send_uid;

    bool ASIC_initalized;
    bool psram_is_available;
    float thermal_throttle_temp;
    float thermal_shutdown_temp;
} GlobalState;

#endif /* GLOBAL_STATE_H_ */
