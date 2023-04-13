/**
 * @author zhouyushuang
 * @date 2023/04/02
 */

#include <iostream>
#include <unistd.h>

#include "singleton.h"
#include "src/crontab.h"
#include "src/execute.h"

/**
 * @brief 复杂任务使用自定义类实现
 *
 */
class TestCrontab : public utils::crontab::Crontab {
public:
    TestCrontab(time_t execute_time, time_t period)
        : Crontab(execute_time, period) {}
    void execute_plan() override {
        std::cout << "this is TestCrontab." << std::endl;
    }
};

typedef Singleton<utils::crontab::CExecute> TSingletonExecute;

int main(int argc, char* argv[]) {
    // 小任务可在业务模块，用lambda表达式注册到定时任务中
    auto plan = []() -> void { std::cout << "Hello world!" << std::endl; };
    auto planend = []() -> bool { return false; };
    DEF_GET_INSTANCE(TSingletonExecute)
        .register_crontab(
            plan, planend, utils::crontab::Crontab::EN_IMMEDIATELY_EXECUTE, 5);

    DEF_GET_INSTANCE(TSingletonExecute)
        .register_crontab(std::make_shared<TestCrontab>(
            utils::crontab::Crontab::EN_IMMEDIATELY_EXECUTE, 5));

    // TODO: 设置全局线程运行标记
    DEF_GET_INSTANCE(TSingletonExecute).set_runnig([]() -> bool {
        return true;
    });

    DEF_GET_INSTANCE(TSingletonExecute).execute();
    return 0;
}
