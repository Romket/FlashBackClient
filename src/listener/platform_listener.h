#pragma once

#if defined(__linux__)
#include "inotify/inotify_listener.h"
namespace FlashBackClient
{
    using PlatformListener = InotifyListener;
} // namespace FlashBackClient
#else
#error "Unsupported platform"
#endif
