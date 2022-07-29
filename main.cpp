#include "classes/mainwindow.h"

#include <QApplication>
/*
#include <discord.h>
#include <csignal>

using namespace discord;

struct DiscordState {
    std::unique_ptr<discord::Core> core;
};

namespace {
    volatile bool interrupted{ false };
}

void initDiscord() {
    const ClientId appId = 1002325872545644574;
    DiscordState state{};
    discord::Core* core{};
    auto response = discord::Core::Create(appId, DiscordCreateFlags_Default, &core);
    state.core.reset(core);

    if (!state.core) {
        std::cout << "Failed to instantiate Discord!";
        std::exit(-1);
    }

    discord::Activity activity{};
    activity.SetDetails("Fruit Tarts");
    activity.SetState("Pop Snacks");
    activity.GetAssets().SetSmallImage("rbxstudio");
    activity.GetAssets().SetSmallText("i mage");
    activity.GetAssets().SetLargeImage("the");
    activity.GetAssets().SetLargeText("u mage");
    activity.SetType(discord::ActivityType::Playing);
    state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        std::cout << ((result == discord::Result::Ok) ? "Succeeded" : "Failed")
            << " updating activity!\n";
    });

    std::signal(SIGINT, [](int) {
        interrupted = true;
    });
    do {
        state.core->RunCallbacks();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    } while (!interrupted);
}
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //initDiscord();
    return a.exec();
}
