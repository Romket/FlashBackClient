#pragma once

#include <flashbackclient/listener.h>

#include <string>
#include <unordered_map>

namespace FlashBackClient
{
    class InotifyListener : public FileChangeListener
    {
    public:
        InotifyListener() : _inotifyFd(-1), FileChangeListener() {}
        virtual ~InotifyListener() = default;

        bool Initialize() override;
        bool Shutdown() override;

        bool AddListener(const std::filesystem::path& path, int depth = 0) override;

    private:
        void processEvents() override;

        int _inotifyFd;
        // cppcheck-suppress unusedStructMember
        std::unordered_map<int, std::string> _watchDescriptors;
    };
} //namespace FlashBackClient
