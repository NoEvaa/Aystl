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

namespace iin {
template <typename T>
struct Singleton {
private:
    Singleton() = default;
    ~Singleton() = default;
public:
    static T & instance() {
        static T s_ins;
        return s_ins;
    }
};

/**
 * Its instance will be created before main() begins.
 * 
 * Refer to: boost/container/detail/singleton.hpp
 */
template <typename T>
struct PreMainSingleton {
private:
    PreMainSingleton() = default;
    ~PreMainSingleton() = default;

    struct instance_creator {
        instance_creator() { PreMainSingleton<T>::instance(); }
        void doNothing() const {}
    };
    static instance_creator s_create_instance;
public:
    static T & instance() {
        static T s_ins;
        s_create_instance.doNothing();
        return s_ins;
    }
};
template <typename T>
typename PreMainSingleton<T>::instance_creator
PreMainSingleton<T>::s_create_instance;
}

