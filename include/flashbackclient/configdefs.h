/**
 * @file configdefs.h
 * @author Chase Attebury (Appleberry) (chaseappleberryboi@gmail.com)
 * @brief Defines default global, target, and scheduler configs
 * @version 0.1
 * @date 2025-04-01
 *
 * @see configs.h
 * @see configs.cpp
 *
 * @copyright Copyright (c) 2025 Luke Houston
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#define DEFAULT_GLOBAL_CONFIG \
    R"(# ~/.config/flashbackclient/flashbackclient.yaml
# Default settings

encrypt: "none"

keep_on_target_move: false
)"

#define DEFAULT_TARGET_CONFIG \
    R"(# Example target file defining a target named example

name: "example"

path: "~/Dev/FlashBackClient"

target_type: "directory"

encrypt: "directory"

path_ignores:
  - ignore: ".cache"
  - ignore: "build"
  - ignore: "lib"
  - ignore: ".git"
  - ignore: ".vscode"
  - ignore: "*.o"

# Rules for triggering actions
rules:
  - id: 0
    name: "Sync files 30 minutes after last file change"
    action: "sync_files"
    cases:
      - id: "on_file_change"
      - id: "after_interval"
        file_change_trigger: true
        after_last: 30
        before_next_scheduled: 30
)"

#define DEFAULT_SCHEDULER_CONFIG \
    R"(
# ~/.config/flashbackclient/scheduler/scheduler.yaml
# Default rules for the scheduler

rules:
  - id: 0
    name: "Download on startup"
    action: "download_changed"
    cases:
      - id: "on_startup"

  - id: 1
    name: "Upload on shutdown"
    action: "upload_changed"
    cases:
      - id: "on_shutdown"

  - id: 2
    name: "Sync changed files after 2 hours"
    action: "sync_files"
    cases:
      - id: "on_file_change"
      - id: "after_interval"
        file_change_trigger: true
        after_last: 120
        before_next_scheduled: 60
)"