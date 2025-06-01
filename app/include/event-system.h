/** 
 * @file sdl-event-observer.h
 * @author yazilimperver (yazilimperver@gmail.com)
 * @brief 
 * @date 2025-05-31
 */
#pragma once

#include <vector>
#include <algorithm>

#include <SDL3/SDL.h>

/** 
 * @brief SDL olaylarını dinlemek isteyen sınıflar için kullanılacak olan soyut sınıftır.
 */
class EventObserver {
public:
    virtual ~EventObserver() = default;
    virtual void OnEvent(const SDL_Event& event) = 0;
};

/** 
 * @brief SDL olaylarını almak isteyen ve kayıtlı olan sınıfların kayıt olduğu ve olayları ileten sınıftır.
 *        Observer tasarım desenini uygular.
 * @ref   https://refactoring.guru/design-patterns/observer/cpp/example
 */
class EventSubject {
private:
    std::vector<EventObserver*> mObservers;

public:
    void AddObserver(EventObserver* observer);    
    void RemoveObserver(EventObserver* observer);    
    void NotifyObservers(const SDL_Event& event);
};