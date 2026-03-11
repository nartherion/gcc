#pragma once

#define NARTHERION_GCC_DELETE_COPY(type) \
    type(const type&) = delete;          \
    type& operator=(const type&) = delete;

#define NARTHERION_GCC_DELETE_MOVE(type) \
    type(type&&) = delete;               \
    type& operator=(type&&) = delete;

#define NARTHERION_GCC_DELETE_COPY_AND_MOVE(type) \
    NARTHERION_GCC_DELETE_COPY(type)              \
    NARTHERION_GCC_DELETE_MOVE(type)