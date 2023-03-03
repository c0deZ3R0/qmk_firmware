// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once


#include "callbacks.h"

// clang-format off
typedef union {
    uint32_t raw;
    struct {

        bool    nuke_switch          :1;

    };
} userspace_config_t;
// clang-format on

extern userspace_config_t userspace_config;