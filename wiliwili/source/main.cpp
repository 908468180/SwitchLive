#include <borealis.hpp>
#include "activity/main_activity.hpp"
#include "api/live_api.hpp"

int main(int argc, char* argv[]) {
    brls::Logger::setLogLevel(brls::LogLevel::DEBUG);

    if (!brls::Application::init()) {
        return EXIT_FAILURE;
    }

    brls::Application::getWindow()->setTitle("SwitchLive");

    live::LiveAPI::getInstance().init();

    brls::Application::pushActivity(new live::MainActivity());

    while (brls::Application::mainLoop()) {
    }

    brls::Application::quit();
    return EXIT_SUCCESS;
}
