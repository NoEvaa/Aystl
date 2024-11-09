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

#define AY_CAT(a, b)   AY_CAT_IMPL(a, b)
#define AY_STR(s)      AY_STR_IMPL(s)
#define AY_EXTNED(...) AY_EXTEND_IMPL(__VA_ARGS__)
#define AY_EMPTY(...)  AY_EMPTY_IMPL(__VA_ARGS__)

#define AY_CAT_IMPL(a, b)   a##b
#define AY_STR_IMPL(a)      #a
#define AY_EXTEND_IMPL(...) __VA_ARGS__
#define AY_EMPTY_IMPL(...)

