#include "event-system.h"

void EventSubject::AddObserver(EventObserver* observer) {
    if (observer != nullptr) {
        mObservers.push_back(observer);
    }
}
    
void EventSubject::RemoveObserver(EventObserver* observer) {
    mObservers.erase(
        std::remove(mObservers.begin(), mObservers.end(), observer),
        mObservers.end()
    );
}
    
void EventSubject::NotifyObservers(const SDL_Event& event) {
    for (auto* observer : mObservers) {
        observer->OnEvent(event);
    }
}