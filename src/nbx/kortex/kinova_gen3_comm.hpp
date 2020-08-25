// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_KINOVA_GEN3_COMM_HPP
#define ROBIF2B_KINOVA_GEN3_COMM_HPP

// Kinova Kortex
#include <BaseClientRpc.h>
#include <BaseCyclicClientRpc.h>
#include <ActuatorConfigClientRpc.h>

#include <SessionManager.h>


namespace k_api = Kinova::Api;


struct robif2b_kinova_gen3_comm
{
    k_api::ITransportClient *transport_tcp;
    k_api::ITransportClient *transport_udp;
    k_api::IRouterClient *router_tcp;
    k_api::IRouterClient *router_udp;
    k_api::SessionManager *session_manager_tcp;
    k_api::SessionManager *session_manager_udp;
    k_api::Base::BaseClient *base;
    k_api::BaseCyclic::BaseCyclicClient *base_cyclic;
    k_api::ActuatorConfig::ActuatorConfigClient *actuator_config;
    k_api::BaseCyclic::Command command;
    k_api::BaseCyclic::Feedback feedback;
    k_api::Base::ServoingModeInformation servoing_mode;
};

#endif
