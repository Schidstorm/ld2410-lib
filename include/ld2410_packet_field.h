#pragma once

#include <cstdint>
#include <ostream>

namespace ld2410 {

enum class FieldName: uint8_t {
    empty,
    value,
    targetState,
    tail,
    status,
    stationaryTargetEnergyValue,
    stationaryTargetDistance,
    staticSensitivityWord,
    staticSensitivityValue,
    sectionUnattendedDuration,
    protocol_version,
    noTimeDuration,
    noPersonDuration,
    movementTargetDistance,
    motionSensitivityWord,
    motionSensitivityValue,
    minorVersionNumber,
    maximumStaticDistanceDoorWord,
    maximumStaticDistanceDoorParameter,
    maximumMovingDistanceWord,
    maximumMovingDistanceParameter,
    majorVersionNumber,
    header,
    firmwareType,
    exerciseTargetEnergyValue,
    distanceGateWord,
    distanceGateValue,
    distanceGateRestSensitivity,
    distanceGateMotionSensitivity,
    detectionDistance,
    check,
    buffer,
    baudRateSelectionIndex,
    N,
    maximumMovingDistanceGateN,
    maximumStaticDistanceGateN,
    movementDistanceGateEnergyValue,
    staticDistanceGateEnergyValue,
};

std::ostream& operator<<(std::ostream& os, const FieldName name) {
    switch(name) {
        case FieldName::empty: os << "empty"; break;
        case FieldName::value: os << "value"; break;
        case FieldName::targetState: os << "targetState"; break;
        case FieldName::tail: os << "tail"; break;
        case FieldName::status: os << "status"; break;
        case FieldName::stationaryTargetEnergyValue: os << "stationaryTargetEnergyValue"; break;
        case FieldName::stationaryTargetDistance: os << "stationaryTargetDistance"; break;
        case FieldName::staticSensitivityWord: os << "staticSensitivityWord"; break;
        case FieldName::staticSensitivityValue: os << "staticSensitivityValue"; break;
        case FieldName::sectionUnattendedDuration: os << "sectionUnattendedDuration"; break;
        case FieldName::protocol_version: os << "protocol_version"; break;
        case FieldName::noTimeDuration: os << "noTimeDuration"; break;
        case FieldName::noPersonDuration: os << "noPersonDuration"; break;
        case FieldName::movementTargetDistance: os << "movementTargetDistance"; break;
        case FieldName::motionSensitivityWord: os << "motionSensitivityWord"; break;
        case FieldName::motionSensitivityValue: os << "motionSensitivityValue"; break;
        case FieldName::minorVersionNumber: os << "minorVersionNumber"; break;
        case FieldName::maximumStaticDistanceDoorWord: os << "maximumStaticDistanceDoorWord"; break;
        case FieldName::maximumStaticDistanceDoorParameter: os << "maximumStaticDistanceDoorParameter"; break;
        case FieldName::maximumMovingDistanceWord: os << "maximumMovingDistanceWord"; break;
        case FieldName::maximumMovingDistanceParameter: os << "maximumMovingDistanceParameter"; break;
        case FieldName::majorVersionNumber: os << "majorVersionNumber"; break;
        case FieldName::header: os << "header"; break;
        case FieldName::firmwareType: os << "firmwareType"; break;
        case FieldName::exerciseTargetEnergyValue: os << "exerciseTargetEnergyValue"; break;
        case FieldName::distanceGateWord: os << "distanceGateWord"; break;
        case FieldName::distanceGateValue: os << "distanceGateValue"; break;
        case FieldName::distanceGateRestSensitivity: os << "distanceGateRestSensitivity"; break;
        case FieldName::distanceGateMotionSensitivity: os << "distanceGateMotionSensitivity"; break;
        case FieldName::detectionDistance: os << "detectionDistance"; break;
        case FieldName::check: os << "check"; break;
        case FieldName::buffer: os << "buffer"; break;
        case FieldName::baudRateSelectionIndex: os << "baudRateSelectionIndex"; break;
        case FieldName::N: os << "N"; break;
        case FieldName::maximumMovingDistanceGateN: os << "maximumMovingDistanceGateN"; break;
        case FieldName::maximumStaticDistanceGateN: os << "maximumStaticDistanceGateN"; break;
        case FieldName::movementDistanceGateEnergyValue: os << "movementDistanceGateEnergyValue"; break;
        case FieldName::staticDistanceGateEnergyValue: os << "staticDistanceGateEnergyValue"; break;
        default: os << "<undefined>"; break;
    }
    return os;
}

class PacketField {
    FieldName m_name;
    uint16_t m_size;

public:
    PacketField(FieldName name, uint16_t size): m_name(name), m_size(size) {

    }

    PacketField(): m_name(FieldName::empty), m_size(0){

    }

    decltype(m_size) size() const {
        return m_size;
    }

    decltype(m_name) name() const {
        return m_name;
    }
};

}