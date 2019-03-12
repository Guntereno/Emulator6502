#pragma once

#define FLAGS_SET(flags, mask) (flags = (flags | mask))
#define FLAGS_UNSET(flags, mask) (flags = (flags & ~mask))
#define FLAGS_SET_TO(flags, mask, val) (val ? FLAGS_SET(flags, mask) : FLAGS_UNSET(flags, mask))
#define FLAGS_CHECK_ALL(flags, check) ((flags & check) == check)
