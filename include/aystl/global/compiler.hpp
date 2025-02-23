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
 * The compiler, must be one of: `AY_CC_x`
 * 
 * The `x` can be:
 *     MSVC  - Microsoft Visual C/C++
 *     GNU   - GNU C++
 *     CLANG - C++ front-end for the LLVM compiler
 *     NVCC  - NVIDIA CUDA C++ compiler for GPU
 */

#if defined(_MSC_VER)
#    define AY_CC_MSVC 1
#endif

#if defined(__GNUC__)
#    define AY_CC_GCC 1
#endif

#if defined(__clang__)
#    define AY_CC_CLANG 1
#endif

#if defined(__CUDACC__)
#    define AY_CC_NVCC 1
#endif

