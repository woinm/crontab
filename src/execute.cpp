#include "execute.h"
#include "crontab.h"
#include <mutex>

namespace utils {
namespace crontab {

CExecute::CExecute() {}

void CExecute::execute() {
    std::unique_lock<std::mutex> lock(m_mutex);
    CrontabPtrSet::iterator it = m_crontabs.begin();
    for (; it != m_crontabs.end();) {
        CrontabPtr plan = (*it);
        if (!plan->is_expire()) {
            ++it;
            continue;
        }
        // TODO: post to thread pools
        plan->execute_plan();
        plan->is_end() ? it = m_crontabs.erase(it) : ++it;
    }
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