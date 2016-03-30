//
// Created by Darth on 29-Mar-16.
//

#ifndef SPACE_RUSH_ANDROIDDEFS_H
#define SPACE_RUSH_ANDROIDDEFS_H

#if __cplusplus < 201402L

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif

#endif //SPACE_RUSH_ANDROIDDEFS_H
