#include <iostream>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>

struct Event {
    int type;
    int priority;
    std::function<void()> handler;
};

struct EventComparator {
    bool operator()(const Event& lhs, const Event& rhs) const {
        return lhs.priority < rhs.priority;
    }
};

class EventDispatcher {
public:
    void registerEventHandler(int eventType, int priority, std::function<void()> handler) {
        std::lock_guard<std::mutex> lock(mutex);
        Event event{ eventType, priority, handler };
        eventQueue.push(event);
    }

    void startDispatcher() {
        dispatcherThread = std::thread(&EventDispatcher::dispatchEvents, this);
    }

    void joinDispatcherThread() {
        dispatcherThread.join();
    }

private:
    void dispatchEvents() {
        while (true) {
            std::lock_guard<std::mutex> lock(mutex);
            if (eventQueue.empty()) {
                break; 
            }

            Event event = eventQueue.top();
            eventQueue.pop();

            event.handler();

            
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

private:
    std::priority_queue<Event, std::vector<Event>, EventComparator> eventQueue;
    std::mutex mutex;
    std::thread dispatcherThread;
};

int main() {
    EventDispatcher eventDispatcher;

    eventDispatcher.registerEventHandler(1, 2, []() {
        std::cout << "Handling event type 1 with priority 2\n";
    });

    eventDispatcher.registerEventHandler(2, 1, []() {
        std::cout << "Handling event type 2 with priority 1\n";
    });

    eventDispatcher.registerEventHandler(1, 1, []() {
        std::cout << "Handling event type 1 with priority 1\n";
    });

    eventDispatcher.startDispatcher();
    eventDispatcher.joinDispatcherThread();

    return 0;
}
