/**
 * @author zhouyushuang
 * @date 2017/09/011
 */

#ifndef TIMER_H
#define TIMER_H

#include "crontab.h"

#include <mutex>
#include <set>
#include <time.h>

namespace utils {
namespace crontab {

typedef std::set<CrontabPtr> CrontabPtrSet;

/**
 * @brief The CExecute class 用于扫描可执行任务，任务最小执行粒度为1s
 */
class CExecute {
    /**
     * @brief The CSoftwareReport class
     * 可定制定时任务派生类，可自由注册定时任务回调函数
     */
    class CFreeCrontab : public Crontab {
    public:
        CFreeCrontab(PlanCB plan_cb,
                     PlanEndCB end_cb,
                     time_t execute_time,
                     time_t period)
            : Crontab(execute_time, period),
              m_plan_cb(plan_cb),
              m_is_end_cb(end_cb) {}

        /**
         * @brief execute_plan
         */
        void execute_plan() {
            if (nullptr != m_plan_cb) {
                m_plan_cb();
            }
        }

        /**
         * @brief is_end
         * @return
         */
        bool is_end() {
            if (nullptr == m_is_end_cb || nullptr == m_plan_cb) {
                return true;
            }
            return m_is_end_cb();
        }

    private:
        /// @brief 定时任务回调
        PlanCB m_plan_cb;
        /// @brief 检查任务状态回调
        PlanEndCB m_is_end_cb;
    };
    typedef std::shared_ptr<CFreeCrontab> CCFreeCrontabPtr;

public:
    /**
     * @brief CExecute 构造函数
     */
    CExecute();

    /**
     * @brief execute 扫描可执行任务，并将可执行任务post到线程池中执行
     */
    virtual void execute();

    /**
     * @brief register_crontab  注册可执行任务
     * @brief register_crontab  定制定时注册定时任务回调函数
     * @brief unregister_crontab 注销可执行任务
     * @param plan
     * @return
     */
    bool register_crontab(CrontabPtr plan);
    bool register_crontab(std::function<void()> plan,
                          std::function<bool()> is_end,
                          time_t execute_time,
                          time_t period);
    void unregister_crontab(CrontabPtr plan);

private:
    std::mutex m_mutex;
    /// @brief 存放任务执行对象
    std::set<CrontabPtr> m_crontabs;
};

} // namespace crontab
} // namespace utils

#endif // TIMER_H
