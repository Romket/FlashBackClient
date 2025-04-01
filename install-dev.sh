#!/bin/bash

# Set project root directory
PROJECT_ROOT=$(pwd)

# Set build type (e.g., Debug, Release)
BUILD_TYPE="Debug"

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

# Create systemd service
install() {
    log "Creating systemd service..."
    cat > "/etc/systemd/system/flashback.client.service" << EOF
[Unit]
Description=Client for FlashBack backup and sync software

[Service]
ExecStart=/usr/local/bin/flashback/client
Restart=always

[Install]
WantedBy=multi-user.target
Ailias=flashback.client.service
EOF

    sudo systemctl enable flashback.client.service
    sudo systemctl start flashback.client.service

    check_error

    log "Installed"
}

main() {
    cmake_build

    install
}

main
