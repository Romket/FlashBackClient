/**
 * @file crashfilesink.h
 * @author Chase Attebury (Appleberry) (chaseappleberryboi@gmail.com)
 * @brief Standard file sink except for delayed file creation
 * @version 0.1
 * @date 2025-03-31
 *
 * @sa logger.h
 * @sa logger.cpp
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

#pragma once

#include <string>

#include <flashbackclient/defs.h>

#include <spdlog/details/file_helper.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace spdlog
{
    namespace sinks
    {
        template<typename Mutex>
        class CrashFileSink : public base_sink<Mutex>
        {
        public:
            explicit CrashFileSink(
                bool                       truncate       = false,
                const file_event_handlers& event_handlers = {});
            const filename_t& filename() const;
            void              truncate();

        protected:
            void sink_it_(const details::log_msg& msg) override;
            void flush_() override;

        private:
            details::file_helper file_helper_;
            bool                 truncate_;
            bool                 initialized_;
        };

        template<typename Mutex>
        SPDLOG_INLINE CrashFileSink<Mutex>::CrashFileSink(
            bool truncate, const file_event_handlers& event_handlers) :
            file_helper_ {event_handlers}, truncate_ {truncate}
        {
        }

        template<typename Mutex>
        SPDLOG_INLINE const filename_t& CrashFileSink<Mutex>::filename() const
        {
            return file_helper_.filename();
        }

        template<typename Mutex>
        SPDLOG_INLINE void CrashFileSink<Mutex>::truncate()
        {
            std::lock_guard<Mutex> lock(base_sink<Mutex>::mutex_);
            file_helper_.reopen(true);
        }

        template<typename Mutex>
        SPDLOG_INLINE void
        CrashFileSink<Mutex>::sink_it_(const details::log_msg& msg)
        {
            memory_buf_t formatted;
            base_sink<Mutex>::formatter_->format(msg, formatted);
            if (!initialized_)
            {
                std::time_t        t  = std::time(nullptr);
                std::tm            tm = *std::localtime(&t);
                std::ostringstream oss;
                oss << std::put_time(&tm, LOG_FILE_FMT);

                std::string _crashFilePath = LOG_DIR + '/' + oss.str();

                file_helper_.open(_crashFilePath, truncate_);
                initialized_ = true;
            }
            file_helper_.write(formatted);
        }

        template<typename Mutex>
        SPDLOG_INLINE void CrashFileSink<Mutex>::flush_()
        {
            file_helper_.flush();
        }
    } // namespace sinks
} // namespace spdlog
