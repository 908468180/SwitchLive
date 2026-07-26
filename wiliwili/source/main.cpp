#include <borealis.hpp>
#include "activity/main_activity.hpp"
#include "api/live_api.hpp"

int main(int argc, char* argv[]) {
    brls::Logger::setLogLevel(brls::LogLevel::LOG_INFO);

    if (!brls::Application::init()) {
        return EXIT_FAILURE;
    }

    brls::Application::createWindow("SwitchLive");

    live::LiveAPI::getInstance().init();

    brls::Application::pushActivity(new live::MainActivity());

    while (brls::Application::mainLoop()) {
    }

    brls::Application::quit();
    return EXIT_SUCCESS;
}
