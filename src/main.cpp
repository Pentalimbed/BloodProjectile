#include <spdlog/sinks/basic_file_sink.h>

#include "re.h"
#include "PrecisionAPI.h"
#include "config.h"

#define DBGMSG

namespace blood
{
bool installLog()
{
    auto path = logger::log_directory();
    if (!path)
        return false;

    *path /= fmt::format(FMT_STRING("{}.log"), SKSE::PluginDeclaration::GetSingleton()->GetName());
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

    auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef DBGMSG
    log->set_level(spdlog::level::info);
    log->flush_on(spdlog::level::info);
#else
    log->set_level(spdlog::level::trace);
    log->flush_on(spdlog::level::trace);
#endif

    spdlog::set_default_logger(std::move(log));
    spdlog::set_pattern("[%H:%M:%S:%e][%5l] %v"s);

    return true;
}

void initPrecisionAPI()
{
    auto result = reinterpret_cast<PRECISION_API::IVPrecision1*>(PRECISION_API::RequestPluginAPI(PRECISION_API::InterfaceVersion::V1));
    if (result)
    {
        logger::info("Obtained PrecisionAPI");
        auto res = result->AddPreHitCallback(
            SKSE::GetPluginHandle(),
            [](const PRECISION_API::PrecisionHitData& precision_hitdata) -> PRECISION_API::PreHitCallbackReturn { return {}; });
        if (res == PRECISION_API::APIResult::OK || res == PRECISION_API::APIResult::AlreadyRegistered)
            logger::info("Collision prehit callback successfully registered.");
    }
    else
        logger::info("Precision API not found.");
}

void processMessage(SKSE::MessagingInterface::Message* a_msg)
{
    switch (a_msg->type)
    {
        case SKSE::MessagingInterface::kDataLoaded:
            logger::info("Game: data loaded");

            logger::info("Loading Config");
            Config::getSingleton()->loadFile();

            logger::info("Installing Hooks");
            stl::write_thunk_call<ProcessHitHook>();
            stl::write_thunk_call<UpdateHook>();

            break;
        case SKSE::MessagingInterface::kPostLoadGame:
            logger::debug("Game: save loaded");



            break;
        default:
            break;
    }
}
} // namespace blood

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    using namespace blood;

    installLog();

    auto* plugin  = SKSE::PluginDeclaration::GetSingleton();
    auto  version = plugin->GetVersion();
    logger::info("{} {} is loading...", plugin->GetName(), version);

    SKSE::Init(skse);
    SKSE::AllocTrampoline(14 * 2);

    auto messaging = SKSE::GetMessagingInterface();
    if (!messaging->RegisterListener("SKSE", processMessage))
        return false;

    logger::info("{} has finished loading.", plugin->GetName());
    return true;
}