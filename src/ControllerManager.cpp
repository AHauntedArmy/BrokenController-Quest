#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/XR/InputDevice.hpp"
#include "UnityEngine/XR/InputDevices.hpp"

#include "Photon/Pun/PhotonView.hpp"

#include "ControllerManager.hpp"
    
using namespace UnityEngine;
using namespace UnityEngine::XR;
using namespace GlobalNamespace;
    
ControllerManager::ControllerManager() : rightController(nullptr), leftController(nullptr),
                                         rightHand(nullptr), onlineRightHand(nullptr),
                                         leftHand(nullptr), onlineLeftHand(nullptr),
                                         rightValid(false), leftVaild(false), loaded(false)
{
    //constructor go brrrrrr
}

void ControllerManager::AddPlayer(GorillaLocomotion::Player* player)
{
    if(player == nullptr || loaded) return;

    GameObject* rightObject = player->rightHandTransform->get_gameObject();
    GameObject* leftObject = player->leftHandTransform->get_gameObject();

    rightController = rightObject->GetComponent<Controller*>();
    leftController = leftObject->GetComponent<Controller*>();

    // if for some reason these don't exist
    if(rightController == nullptr || leftController == nullptr){
        rightController = nullptr;
        leftController = nullptr;
        loaded = false;
        return;
    }

    rightValid = rightController->get_inputDevice().get_isValid();
    leftVaild = leftController->get_inputDevice().get_isValid();

    loaded = true;

    UpdateControllers();
}

void ControllerManager::AddVRRig(GlobalNamespace::VRRig* rig)
{
    if(rig == nullptr) return;

    Photon::Pun::PhotonView* rigPhoton = rig->get_photonView();

    if(rigPhoton == nullptr){
        if(rig->isOfflineVRRig){
            rightHand = rig->rightHand;
            leftHand = rig->leftHand;
        }
    }else{
        if(rigPhoton->IsMine)
        {
            onlineRightHand = rig->rightHand;
            onlineLeftHand = rig->leftHand;
        }
    }

    UpdateControllers();
}

void ControllerManager::RemoveVRRig(GlobalNamespace::VRRig* rig)
{
    if(rig == nullptr) return;

    Photon::Pun::PhotonView* rigPhoton = rig->get_photonView();

    if(rigPhoton == nullptr){
        if(rig->isOfflineVRRig){
            rightHand = nullptr;
            leftHand = nullptr;
        }
    }else{
        if(rigPhoton->IsMine){
            onlineRightHand = nullptr;
            onlineLeftHand = nullptr;
        }
    }

    UpdateControllers();
}

void ControllerManager::DeviceConnected()
{
    rightValid = InputDevices::GetDeviceAtXRNode(XRNode::RightHand).get_isValid();
    leftVaild = InputDevices::GetDeviceAtXRNode(XRNode::LeftHand).get_isValid();

    UpdateControllers();
}

void ControllerManager::UpdateControllers()
{
    if(!loaded) return;

    // if both controllers are connected or disconnected, set everything to default
    if(rightValid == leftVaild){
        if(onlineRightHand != nullptr) onlineRightHand->vrTargetNode = XRNode::RightHand;
        if(onlineLeftHand != nullptr) onlineLeftHand->vrTargetNode = XRNode::LeftHand;
        if(rightHand != nullptr) rightHand->vrTargetNode = XRNode::RightHand;
        if(leftHand != nullptr) leftHand->vrTargetNode = XRNode::LeftHand;

        rightController->set_controllerNode(XRNode::RightHand);
        leftController->set_controllerNode(XRNode::LeftHand);
        rightController->m_InputDevice = InputDevices::GetDeviceAtXRNode(XRNode::RightHand);
        leftController->m_InputDevice = InputDevices::GetDeviceAtXRNode(XRNode::LeftHand);

        return;
    }

    if(rightValid){
        if(onlineLeftHand != nullptr) onlineLeftHand->vrTargetNode = XRNode::RightHand;
        if(leftHand != nullptr) leftHand->vrTargetNode = XRNode::RightHand;
        leftController->set_controllerNode(XRNode::RightHand);
        return;
    }

    if(leftVaild){
        if(onlineRightHand != nullptr) onlineRightHand->vrTargetNode = XRNode::LeftHand;
        if(rightHand != nullptr) rightHand->vrTargetNode = XRNode::LeftHand;
        rightController->set_controllerNode(XRNode::LeftHand);
        return;
    }
}