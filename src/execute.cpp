#include "execute.h"
#include "crontab.h"
#include <mutex>
#include <unistd.h>

namespace utils {
namespace crontab {

CExecute::CExecute() {}

void CExecute::execute() {
    auto do_execute = [this]() {
        std::unique_lock<std::mutex> lock(m_mutex);
        CrontabPtrSet::iterator it = m_crontabs.begin();
        for (; it != m_crontabs.end();) {
            CrontabPtr plan = (*it);

            if (plan->is_end()) {
                it = m_crontabs.erase(it);
                continue;
            }

            ++it;
            if (!plan->is_expire()) {
                continue;
            }
            if (nullptr != m_post) {
                m_post(std::bind(&Crontab::execute_plan, plan));
            } else {
                plan->execute_plan();
            }
        }
    };

    do {
        do_execute();
        usleep(500 * 1000);
    } while (m_running());
}

bool CExecute::register_crontab(CrontabPtr plan) {
    std::unique_lock<std::mutex> lock(m_mutex);
    CrontabPtrSet::iterator it = m_crontabs.find(plan);
    if (it == m_crontabs.end()) {
        m_crontabs.insert(plan);
        return true;
    }
    return false;
}

bool CExecute::register_crontab(std::function<void()> plan,
                                std::function<bool()> is_end,
                                time_t execute_time,
                                time_t period) {
    std::unique_lock<std::mutex> lock(m_mutex);
    auto crontab =
        std::make_shared<CFreeCrontab>(plan, is_end, execute_time, period);
    if (nullptr != crontab) {
        m_crontabs.insert(crontab);
        return true;
    }
    return false;
}

void CExecute::unregister_crontab(CrontabPtr plan) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_crontabs.erase(plan);
}

} // namespace crontab
} // namespace utils