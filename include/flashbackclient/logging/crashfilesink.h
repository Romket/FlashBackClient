#pragma once

#include <spdlog/details/file_helper.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace spdlog
{
    namespace sinks
    {
        template <typename Mutex>
        class CrashFileSink : public base_sink<Mutex> {
        public:
            explicit CrashFileSink(const filename_t &filename,
                                    bool truncate = false,
                                    const file_event_handlers &event_handlers = {});
            const filename_t &filename() const;
            void truncate();

        protected:
            void sink_it_(const details::log_msg &msg) override;
            void flush_() override;

        private:
            details::file_helper file_helper_;
            filename_t file_name_;
            bool truncate_;
            bool initialized_;
        };

        template <typename Mutex>
        SPDLOG_INLINE CrashFileSink<Mutex>::CrashFileSink(const filename_t &filename,
                                                            bool truncate,
                                                            const file_event_handlers &event_handlers)
            : file_helper_{event_handlers}, file_name_{filename}, truncate_{truncate} { initialized_ = false;}
        
        template <typename Mutex>
        SPDLOG_INLINE const filename_t &CrashFileSink<Mutex>::filename() const {
            return file_helper_.filename();
        }

        template <typename Mutex>
        SPDLOG_INLINE void CrashFileSink<Mutex>::truncate() {
            std::lock_guard<Mutex> lock(base_sink<Mutex>::mutex_);
            file_helper_.reopen(true);
        }

        template <typename Mutex>
        SPDLOG_INLINE void CrashFileSink<Mutex>::sink_it_(const details::log_msg &msg) {
            memory_buf_t formatted;
            base_sink<Mutex>::formatter_->format(msg, formatted);
            if (!initialized_) 
            {
            file_helper_.open(file_name_, truncate_);
            initialized_ = true;
            }
            file_helper_.write(formatted);
        }

        template <typename Mutex>
        SPDLOG_INLINE void CrashFileSink<Mutex>::flush_() {
            file_helper_.flush();
        }
    } //namespace sinks
} // namespace spdlog