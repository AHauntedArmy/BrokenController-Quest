#pragma once

#include "GorillaLocomotion/Player.hpp"
#include "GlobalNamespace/VRRig.hpp"
#include "GlobalNamespace/VRMap.hpp"

#include "UnityEngine/XR/InputDeviceCharacteristics.hpp"
#include "UnityEngine/XR/Interaction/Toolkit/XRController.hpp"

class ControllerManager{
    private:
        typedef UnityEngine::XR::Interaction::Toolkit::XRController Controller;
        Controller* rightController;
        Controller* leftController;

        typedef GlobalNamespace::VRMap HandMap;
        HandMap* rightHand;
        HandMap* leftHand;
        HandMap* onlineRightHand;
        HandMap* onlineLeftHand;

        bool  rightValid;
        bool leftVaild;

        bool loaded;

    public:
        ControllerManager();
        void AddPlayer(GorillaLocomotion::Player* player);

        void AddVRRig(GlobalNamespace::VRRig* rig);
        void RemoveVRRig(GlobalNamespace::VRRig* rig);

        void DeviceConnected();
        

    private:
        void UpdateControllers();
};