#!/bin/bash

# Set project root directory
PROJECT_ROOT=$(pwd)

# Set build type (e.g., Debug, Release)
BUILD_TYPE="Release"

# Set CMake generator (e.g., Unix Makefiles, Ninja)
GENERATOR="Unix Makefiles"

# Function to log messages
log() {
    echo "$(date): $1"
}

# Function to check for errors
check_error() {
    if [ $? -ne 0 ]; then
        log "Error occurred during build"
        exit 1
    fi
}

# Run CMake
cmake_build() {
    log "Running CMake..."
    cmake                              \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
        -G "$GENERATOR"                \
        -S "${PROJECT_ROOT}"           \
        -B "${PROJECT_ROOT}/build"

    if [ $? -ne 0 ]; then
        log "Failed to configure project"
        exit 2
    fi

    # Execute build command
    case "$GENERATOR" in
        "Unix Makefiles")
            log "Building with Make..."
            cd build
            make
            sudo make install
            cd ..
            ;;
        "Ninja")
            log "Building with Ninja..."
            cd build
            ninja
            cd ..
            ;;
        *)
            log "Unsupported generator: $GENERATOR"
            exit 1
            ;;
    esac

    check_error

    log "Build completed successfully!"
}

# Install scheduler script to /usr/local/bin and set to run on startup
install_scheduler() {
    echo "Installing scheduler..."
    sudo cp scheduler /usr/local/bin/flashbackclient/scheduler
    sudo chmod +x /usr/local/bin/flashbackclient/scheduler

    echo "Creating systemd service..."
    sudo cat > "/etc/systemd/system/flashback.scheduler.service" << EOF
[Unit]
Description=Scheduler for FlashBack

[Service]
ExecStart=/usr/local/bin/flashbackclient/scheduler
Restart=always

[Install]
WantedBy=multi-user.target
Ailias=flashback.scheduler.service
EOF

    systemctl enable flashback.scheduler.service
    systemctl start flashback.scheduler.service

    check_error

    echo "Scheduler installed"
}

# Generate config files
generate_configs() {
    echo "Generating configs..."

    mkdir ~/.flashback
    mkdir ~/.flashback/scheduler

    # TODO: create default config files
    #// cp flashback.conf ~/.flashback/flashback.conf
    #// cp scheduler.conf ~/.flashback/scheduler/scheduler.conf
}

main() {
    cmake_build

    install_scheduler

    generate_configs
}

main
