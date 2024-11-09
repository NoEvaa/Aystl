/**
 * Copyright 2024 NoEvaa
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once
/**
 * The operating system, must be one of: `AY_OS_x`
 * 
 * The `x` can be:
 *     LINUX   - Linux
 *     ANDROID - Android platform
 *     WIN     - Any Windows system
 *     WIN32   - Win32 (Windows 2000/NT/XP/Vista/7 and Windows Server 2003/2008)
 *     WIN64   - Win64
 *     APPLE   - Any Apple system (macOS, iOS, watchOS, tvOS, ...)
 *     MACOS   - macOS
 */

#if (defined(linux) || defined(__linux) || defined(__linux__)                                      \
    || defined(__GNU__) || defined(__GLIBC__)) && !defined(_CRAYC)
#    define AY_OS_LINUX 1
#
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__NT__)
#    define AY_OS_WIN32 1
#
#elif defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#    define AY_OS_WIN64 1
#
#elif defined(__APPLE__) || defined(__APPLE_CC__)
#    define AY_OS_APPLE 1
#    if not __has_include(<TargetConditionals.h>)
#        error "Missing `TargetConditionals.h` on Apple platform."
#    endif
#    include <TargetConditionals.h>
#    if defined(TARGET_OS_MAC) && TARGET_OS_MAC
#        if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#        else
#            define AY_OS_MACOS 1
#        endif
#    endif
#
#else
#    error "Unknown platform."
#endif

#if defined(AY_OS_WIN32) || defined(AY_OS_WIN64)
#    define AY_OS_WIN 1
#endif

#if defined(__ANDROID__) || defined(ANDROID)
#    define AY_OS_ANDROID 1
#endif

