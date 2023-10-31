# Use Ubuntu 22.04 LTS as the base image
FROM ubuntu:22.04

# Set environment variables to suppress debconf warnings
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install build-essential (includes GCC and Make)
# Install valgrind, protobuf-c-compiler, and libprotobuf-c-dev
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    git \
    valgrind \
    protobuf-c-compiler \
    libprotobuf-c-dev && \
    # Clean up the package lists to reduce image size
    rm -rf /var/lib/apt/lists/*

# Set the working directory in the container
WORKDIR /app

# Copy the local directory contents into the container at /app
COPY . /app


