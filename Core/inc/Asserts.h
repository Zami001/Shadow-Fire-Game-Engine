#pragma once

#include <stdlib.h>
#include <exception>
#include <Logging/Log.h>

#define SF_ASSERT_EXIT_CODE 1

#if SF_USE_ASSERTS
#define SF_ASSERT(x, msg) if(!(x)) { SF_LOG(Assert, Fatal, "%s - %s", __func__, msg) throw std::exception(); }
#define SF_ENSURE(x, msg) if(!(x)) { SF_LOG(Assert, Error, "%s - %s", __func__, msg) }
#define SF_NOENTRY() { SF_LOG(Assert, Fatal, "No entry assert hit in file - %s", __func__) throw std::exception(); }
#endif

#ifndef SF_ASSERT
#define SF_ASSERT(x, msg)
#define SF_ENSURE(x, msg)
#define SF_NOENTRY()
#endif