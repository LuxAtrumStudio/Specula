#ifndef SPECULA_GLOBAL_INCLUDE_HPP_
#define SPECULA_GLOBAL_INCLUDE_HPP_

#include "compiler.hpp"
#include "os.hpp"

#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#if SPECULA_OS_IS_Windows || SPECULA_OS_IS_BSD || SPECULA_OS_IS_OSX
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#if SPECULA_COMPILER_IS_MSVC == 1
#include <float.h>
#include <intrin.h>
#endif

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "../dev.hpp"

#endif /* end of include guard: SPECULA_GLOBAL_INCLUDE_HPP_ */
