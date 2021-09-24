#include "main.hpp"

// for device connect hooks
#include "UnityEngine/XR/InputDevices.hpp"
#include "UnityEngine/XR/ConnectionChangeType.hpp"

// for player hooks
#include "GorillaLocomotion/Player.hpp"
#include "GlobalNamespace/VRRig.hpp"

#include "ControllerManager.hpp"

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

ControllerManager controllerManager;

// UnityEngine::XR InputDevices.InvokeConnectionEvent(ulong, ConnectionChangeType)
MAKE_HOOK_MATCH(DeviceConnectEvent, &UnityEngine::XR::InputDevices::InvokeConnectionEvent, void, uint64_t deviceId, UnityEngine::XR::ConnectionChangeType change)
{
    DeviceConnectEvent(deviceId, change);

    typedef UnityEngine::XR::ConnectionChangeType ChangeType;
    switch(change.value){
        case ChangeType::Disconnected : 
        case ChangeType::Connected : {
            controllerManager.DeviceConnected();
            break;
        }
    }

}

//GorillaLocomotion Player.Awake()
MAKE_HOOK_MATCH(PlayerAwake, &GorillaLocomotion::Player::Awake, void, GorillaLocomotion::Player* self)
{
    PlayerAwake(self);
    controllerManager.AddPlayer(self);
}

//VRRig.Start()
MAKE_HOOK_MATCH(VRRigSpawned, &GlobalNamespace::VRRig::Start, void, GlobalNamespace::VRRig* self)
{
    VRRigSpawned(self);
    controllerManager.AddVRRig(self);
}

//VRRig.OnDestroy()
MAKE_HOOK_MATCH(VRRigDespawned, &GlobalNamespace::VRRig::OnDestroy, void, GlobalNamespace::VRRig* self)
{
    VRRigDespawned(self);
    controllerManager.RemoveVRRig(self);
}

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    Logger& logger = getLogger();

    logger.info("Installing hooks...");

    // Install our hooks
    INSTALL_HOOK(logger, DeviceConnectEvent);
    INSTALL_HOOK(logger, PlayerAwake);
    INSTALL_HOOK(logger,VRRigSpawned);
    INSTALL_HOOK(logger, VRRigDespawned);

    logger.info("Installed all hooks!");
}