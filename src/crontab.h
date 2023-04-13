/**
 * @author zhouyushuang
 * @date 2017/09/011
 */
#ifndef CRONTAB_H
#define CRONTAB_H

#include <functional>
#include <memory>
#include <time.h>

namespace utils {
namespace crontab {

#ifndef DURATION
#define DURATION(t1, t2) (std::abs((t1) - (t2)))
#endif

/**
 * @brief The CExecutePlanBase class
 * 计划任务基类，负责检查是否满足任务执行条件，并提供执行任务接口
 */
class Crontab {
public:
    typedef std::function<void()> PlanCB;
    typedef std::function<bool()> PlanEndCB;

public:
    enum { EN_IMMEDIATELY_EXECUTE, EN_DELAY_EXECUTE };

    Crontab(time_t execute_time,
            time_t period /*, ExecutePlanPtr base_ptr, CExecute *execute*/)
        : m_execute_time(execute_time), m_period(period), m_is_end(false) {
        m_execute_time =
            (EN_IMMEDIATELY_EXECUTE == execute_time ? 0 : time(nullptr));
    }

    virtual ~Crontab() {}

    /**
     * @brief is_expire 检查是否满足任务执行条件
     * @return
     */
    virtual bool is_expire() {
        auto now = time(nullptr);
        if (DURATION(now, m_execute_time) >= m_period) {
            m_execute_time = now;
            return true;
        }
        return false;
    }

    /**
     * @brief set_execute_time
     * @param execute_time
     */
    inline void set_execute_time(time_t execute_time) {
        m_execute_time = execute_time;
    }

    /**
     * @brief set_period
     * @param period
     */
    inline void set_period(time_t period) {
        m_period = (period > 1 ? period : 1);
    }

protected:
    /// @brief 该任务的最后执行时间
    time_t m_execute_time;
    /// @brief 任务执行周期
    time_t m_period;
    /// @brief 任务结束标记
    bool m_is_end;

public:
    /**
     * @brief perform_timer 任务执行接口
     */
    virtual void execute_plan() = 0;

    /**
     * @brief is_end
     * @return
     */
    virtual bool is_end() {
        return m_is_end;
    }
};

typedef std::shared_ptr<Crontab> CrontabPtr;

#define MAKE_CRONTAB(CLASS, FLAG, PERIOD)                                      \
    std::make_shared<CLASS>(edr3::core::FLAG, PERIOD)

} // namespace crontab
} // namespace utils
#endif // CRONTAB_H
