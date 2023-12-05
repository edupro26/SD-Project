# Use Ubuntu 22.04 LTS as the base image
FROM ubuntu:22.04

# Set environment variables to suppress debconf warnings
ENV DEBIAN_FRONTEND=noninteractive

# Update and install dependencies


RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    valgrind \
    libzookeeper-mt-dev \
    protobuf-c-compiler \
    default-jre \
    wget \
    net-tools \
    libprotobuf-c-dev && \
    # Clean up the package lists to reduce image size
    rm -rf /var/lib/apt/lists/*


# Set the working directory in the container
WORKDIR /app


# Set the working directory in the container
WORKDIR /app

# Download and extract Zookeeper, then move the configuration file
RUN wget --no-check-certificate https://archive.apache.org/dist/zookeeper/zookeeper-3.5.9/apache-zookeeper-3.5.9-bin.tar.gz && \
    tar xvzf apache-zookeeper-3.5.9-bin.tar.gz && \
    mv apache-zookeeper-3.5.9-bin zk && \
    mv zk/conf/zoo_sample.cfg zk/conf/zoo.cfg

ENV PATH="/app/zk/bin:${PATH}"

# Copy the local directory contents into the container at /app
COPY . /app




# Copy the local directory contents into the container at /app
COPY . /app


