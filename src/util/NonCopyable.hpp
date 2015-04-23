#pragma once

#define NONCOPYABLE(cls)      \
    cls(const cls&) = delete; \
    const cls& operator=(const cls&) const = delete
