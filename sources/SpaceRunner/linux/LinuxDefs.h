//
// Created by darth on 3/30/16.
//

#ifndef LINUX_LINUXDEFS_H
#define LINUX_LINUXDEFS_H

#if __cplusplus < 201402L

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif

#endif //LINUX_LINUXDEFS_H
