//
// chobo-log
// Copyright (c) 2016-2018 Chobolabs Inc.
// http://www.chobolabs.com/
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// Main configuration file

////////////////////////////////////////////////////////////////////////////////
// Dynamic library interface

#if !defined(CHOBO_SYMBOL_IMPORT)
#   if defined(_MSC_VER)
#       define CHOBO_SYMBOL_EXPORT __declspec(dllexport)
#       define CHOBO_SYMBOL_IMPORT __declspec(dllimport)
#   else
#       define CHOBO_SYMBOL_EXPORT __attribute__((__visibility__("default")))
#       define CHOBO_SYMBOL_IMPORT
#   endif
#endif

#if defined(CHOBO_LOG_DYN_LINK) || defined(CHOBO_ALL_LIBS_DYN_LINK)
#   if defined(_CHOBO_LOG_SRC)
#       define CHOBO_LOG_API CHOBO_SYMBOL_EXPORT
#   else
#       define CHOBO_LOG_API CHOBO_SYMBOL_IMPORT
#   endif
#else
#   define CHOBO_LOG_API
#endif
