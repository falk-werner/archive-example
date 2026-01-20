FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && apt install -y --no-install-recommends \
    ca-certificates \
    git \
    openssh-client \
    gnupg \
    build-essential \
    pkg-config \
    cmake \
    libgtest-dev \
    libgmock-dev \
    valgrind \
    libarchive-dev \
    libzip-dev \
    zip \
    unzip

