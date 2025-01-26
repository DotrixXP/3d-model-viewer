#include <condition_variable>
#include <functional>
#include <future>
#include <queue>
#include <thread>
#include "Assert.hpp"

namespace VxlnUtils {

    class ThreadPool {
      public:
        ThreadPool(uint16_t numThreads) {
            for (auto i = 0; i < numThreads; ++i) {

                // Vytvořit počet vláken podle parametru numThreads
                m_threads.emplace_back([this] {
                    for (;;) {
                        std::unique_lock<std::mutex> lock(this->m_mutex);
                        // Vlákno bude čekat na notify zatímco uvolní lock
                        // Pokud dostane notify, tak se pokusí získat lock a pokračovat
                        this->m_condition.wait(lock, [this] {
                            return this->m_stop || !this->m_tasks.empty();
                        });

                        // Ukončit loop při zavolání destruktoru a dokončení práce
                        if (this->m_stop && this->m_tasks.empty()) {
                            return;
                        }

                        // Vytáhnout task z fronty a spustit ho
                        auto task = std::move(this->m_tasks.front());
                        this->m_tasks.pop();
                        lock.unlock();
                        task();
                    }
                });
            }
        }

        ~ThreadPool() {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_stop = true;
            }
            m_condition.notify_all();
            // Počkat na ukončení všech vláken
            for (std::thread& thread : m_threads) {
                thread.join();
            }
        }

        template <typename F, typename... Args>
        // F je funkce a Args jsou argumenty funkce F
        auto Enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {

            // Zjistit return type funkce pomocí decltype (zjištění typu hodnoty)
            using return_type = decltype(f(args...));

            // Vytvoření tasku, který bude obsahovat funkci a její argumenty
            auto task = std::make_shared<std::packaged_task<return_type()>>(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...));

            std::future<return_type> res = task->get_future();
            {
                // Lock mutexu pro přístup k tasks queue
                std::lock_guard<std::mutex> lock(m_mutex);
                ASSERT(!m_stop, "ThreadPool is stopped and cannot accept new tasks!");
                m_tasks.emplace([task]() {
                    (*task)();
                });
            }
            // Notifikace jednoho vlákna, které čeká na práci
            m_condition.notify_one();
            return res;
        }

      private:
        std::queue<std::function<void()>> m_tasks;
        std::vector<std::thread> m_threads;
        std::mutex m_mutex;

        std::condition_variable m_condition;
        bool m_stop {};
    };

} // namespace VxlnUtils
