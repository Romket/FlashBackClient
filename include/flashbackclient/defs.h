#pragma once

#define HOME std::string(std::getenv("HOME"))

#define CONFIG_DIR HOME + "/.config/flashbackclient"
#define CONFIG_FILE "flashbackclient.yaml"
#define CONFIG_FILE_PATH CONFIG_DIR "/" CONFIG_FILE

#define SCHEDULER_CONFIG_DIR HOME + "/.config/flashbackclient/scheduler"
#define SCHEDULER_CONFIG_FILE "scheduler.yaml"
#define SCHEDULER_CONFIG_FILE_PATH SCHEDULER_CONFIG_DIR "/" SCHEDULER_CONFIG_FILE

#define TARGET_DEF_DIR HOME + "/.config/flashbackclient/targets"

#define LOG_DIR HOME + "/.local/share/flashbackclient"
#define LOG_FILE_FMT "%Y-%m-%d_%H-%M-%S"

#define RECURSION_LIMIT 10

#define THREAD_WAIT_TIME 1000
