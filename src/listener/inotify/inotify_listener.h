#pragma once

#include <flashbackclient/listener.h>

#include <string>

namespace FlashBackClient
{
    class InotifyListener : public FileChangeListener
    {
    public:
        // cppcheck-suppress uninitMemberVar
        InotifyListener() : FileChangeListener() {}
        virtual ~InotifyListener() = default;

        bool Initialize() override;
        bool Shutdown() override;

        bool AddListener(const std::filesystem::path& path) override;

    private:
        void processEvents() override;

        // cppcheck-suppress unusedStructMember
        int _inotifyFd;
        // cppcheck-suppress unusedStructMember
        std::unordered_map<int, std::string> _watchDescriptors;
    };
} //namespace FlashBackClient
