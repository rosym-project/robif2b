// SPDX-License-Identifier: LGPL-3.0
#include <robif2b/functions/kinova_gen3.h>
#include "kinova_gen3_comm.hpp"

#include <cmath>
#include <cassert>

// Kinova Kortex
#include <IRouterClient.h>
#include <RouterClient.h>

#include <ITransportClient.h>
#include <TransportClientUdp.h>
#include <TransportClientTcp.h>


#define DEG_TO_RAD(x) (x) * M_PI / 180.0
#define RAD_TO_DEG(x) (x) * 180.0 / M_PI


void publish_measurement(struct robif2b_kinova_gen3_nbx *b)
{
    assert(b->comm);
    assert(b->jnt_pos_msr);
    assert(b->jnt_vel_msr);
    assert(b->jnt_trq_msr);
    assert(b->act_cur_msr);
    assert(b->imu_ang_vel_msr);
    assert(b->imu_lin_acc_msr);

    robif2b_kinova_gen3_comm *comm = b->comm;

    for (int i = 0; i < ROBIF2B_KINOVA_GEN3_NR_JOINTS; i++) {
        b->jnt_pos_msr[i] = DEG_TO_RAD(comm->feedback.actuators(i).position());
        b->jnt_vel_msr[i] = DEG_TO_RAD(comm->feedback.actuators(i).velocity());
        b->jnt_trq_msr[i] = comm->feedback.actuators(i).torque();
        b->act_cur_msr[i] = comm->feedback.actuators(i).current_motor();
    }

    b->imu_ang_vel_msr[0] = DEG_TO_RAD(comm->feedback.base().imu_angular_velocity_x());
    b->imu_ang_vel_msr[1] = DEG_TO_RAD(comm->feedback.base().imu_angular_velocity_y());
    b->imu_ang_vel_msr[2] = DEG_TO_RAD(comm->feedback.base().imu_angular_velocity_z());
    b->imu_lin_acc_msr[0] = comm->feedback.base().imu_acceleration_x();
    b->imu_lin_acc_msr[1] = comm->feedback.base().imu_acceleration_y();
    b->imu_lin_acc_msr[2] = comm->feedback.base().imu_acceleration_z();
}


void robif2b_kinova_gen3_configure(struct robif2b_kinova_gen3_nbx *b)
{
    assert(b);
    assert(b->jnt_pos_msr);
    assert(b->success);

    b->comm = new robif2b_kinova_gen3_comm;
    if (!b->comm) {
        *b->success = false;
        return;
    }

    robif2b_kinova_gen3_comm *comm = b->comm;
    robif2b_kinova_gen3_config *conf = &b->conf;

    // Create API objects
    auto error_callback = [](k_api::KError err){ std::cout << "_________ callback error _________" << err.toString() << std::endl; };

    std::cout << "Creating transport objects" << std::endl;
    comm->transport_tcp = new k_api::TransportClientTcp();
    comm->router_tcp = new k_api::RouterClient(comm->transport_tcp, error_callback);
    comm->transport_tcp->connect(conf->ip_address, conf->port);

    std::cout << "Creating transport real time objects" << std::endl;
    comm->transport_udp = new k_api::TransportClientUdp();
    comm->router_udp = new k_api::RouterClient(comm->transport_udp, error_callback);
    comm->transport_udp->connect(conf->ip_address, conf->port_real_time);

    // Set session data connection information
    auto create_session_info = k_api::Session::CreateSessionInfo();
    create_session_info.set_username(conf->user);
    create_session_info.set_password(conf->password);
    create_session_info.set_session_inactivity_timeout(conf->session_timeout);
    create_session_info.set_connection_inactivity_timeout(conf->connection_timeout);

    // Session manager service wrapper
    std::cout << "Creating session for communication" << std::endl;
    comm->session_manager_tcp = new k_api::SessionManager(comm->router_tcp);
    comm->session_manager_tcp->CreateSession(create_session_info);
    comm->session_manager_udp = new k_api::SessionManager(comm->router_udp);
    comm->session_manager_udp->CreateSession(create_session_info);
    std::cout << "Session created" << std::endl;

    // Create services
    comm->base = new k_api::Base::BaseClient(comm->router_tcp);
    comm->base_cyclic = new k_api::BaseCyclic::BaseCyclicClient(comm->router_udp);
    comm->actuator_config = new k_api::ActuatorConfig::ActuatorConfigClient(comm->router_tcp);

    // Get first state from the robot
    comm->feedback = comm->base_cyclic->RefreshFeedback();
    publish_measurement(b);

    // Initialize each actuator to its current position
    // Initialize each actuator's control mode
    k_api::ActuatorConfig::ControlModeInformation ctrl_mode_msg = k_api::ActuatorConfig::ControlModeInformation();
    ctrl_mode_msg.set_control_mode(k_api::ActuatorConfig::ControlMode::POSITION);

    for (int i = 0; i < ROBIF2B_KINOVA_GEN3_NR_JOINTS; i++) {
        k_api::BaseCyclic::ActuatorCommand *act = comm->command.add_actuators();
        act->set_position(b->jnt_pos_msr[i]);
        act->set_velocity(0.0);

        // Note that the actuator IDs start at 1
        comm->actuator_config->SetControlMode(ctrl_mode_msg, i + 1);
    }
    b->ctrl_mode_prev = ROBIF2B_CTRL_MODE_VELOCITY;

    *b->success = true;
}


void robif2b_kinova_gen3_shutdown(struct robif2b_kinova_gen3_nbx *b)
{
    assert(b);
    assert(b->success);
    assert(b->comm);

    robif2b_kinova_gen3_comm *comm = b->comm;

    // Close API session
    comm->session_manager_udp->CloseSession();
    comm->session_manager_tcp->CloseSession();

    // Deactivate the router and cleanly disconnect from the transport object
    comm->transport_udp->disconnect();
    comm->router_udp->SetActivationStatus(false);
    comm->transport_tcp->disconnect();
    comm->router_tcp->SetActivationStatus(false);

    // Destroy the API
    delete comm->base;
    delete comm->base_cyclic;
    delete comm->actuator_config;
    delete comm->session_manager_udp;
    delete comm->session_manager_tcp;
    delete comm->router_tcp;
    delete comm->router_udp;
    delete comm->transport_tcp;
    delete comm->transport_udp;
    delete comm;

    *b->success = true;
}


void robif2b_kinova_gen3_start(struct robif2b_kinova_gen3_nbx *b)
{
    assert(b);
    assert(b->success);
    assert(b->comm);

    robif2b_kinova_gen3_comm *comm = b->comm;

    // Set the base in low-level servoing mode
    comm->servoing_mode.set_servoing_mode(k_api::Base::ServoingMode::LOW_LEVEL_SERVOING);
    comm->base->SetServoingMode(comm->servoing_mode);

    *b->success = true;
}


void robif2b_kinova_gen3_stop(struct robif2b_kinova_gen3_nbx *b)
{
    assert(b);
    assert(b->success);
    assert(b->comm);

    robif2b_kinova_gen3_comm *comm = b->comm;

    // Set the servoing mode back to Single Level
    comm->servoing_mode.set_servoing_mode(k_api::Base::ServoingMode::SINGLE_LEVEL_SERVOING);
    comm->base->SetServoingMode(comm->servoing_mode);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Make movement stop
    b->comm->base->Stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    *b->success = true;
}


void robif2b_kinova_gen3_recover(struct robif2b_kinova_gen3_nbx *b)
{
    assert(b);
    assert(b->success);
    assert(b->comm);

    try {
        b->comm->base->ClearFaults();
        *b->success = true;
    } catch (...) {
        *b->success = false;
    }
}


void robif2b_kinova_gen3_update(struct robif2b_kinova_gen3_nbx *b)
{
    assert(b);
    assert(b->ctrl_mode);
    assert(b->comm);
    assert(b->success);
    assert(b->cycle_time);

    robif2b_kinova_gen3_comm *comm = b->comm;


    // Switch control mode
    if (*b->ctrl_mode != b->ctrl_mode_prev) {
        k_api::ActuatorConfig::ControlModeInformation ctrl_mode_msg = k_api::ActuatorConfig::ControlModeInformation();

        switch (*b->ctrl_mode) {
            case ROBIF2B_CTRL_MODE_POSITION:
            case ROBIF2B_CTRL_MODE_VELOCITY:
                ctrl_mode_msg.set_control_mode(k_api::ActuatorConfig::ControlMode::POSITION);
            break;

            case ROBIF2B_CTRL_MODE_FORCE:
                ctrl_mode_msg.set_control_mode(k_api::ActuatorConfig::ControlMode::TORQUE);
            break;

            case ROBIF2B_CTRL_MODE_CURRENT:
                ctrl_mode_msg.set_control_mode(k_api::ActuatorConfig::ControlMode::CURRENT);
            break;

            default:
                robif2b_kinova_gen3_stop(b);
                *b->success = false;
                return;
        }

        for (int i = 0; i < ROBIF2B_KINOVA_GEN3_NR_JOINTS; i++) {
            // Note that the actuator IDs start at 1
            comm->actuator_config->SetControlMode(ctrl_mode_msg, i + 1);
        }
    }


    // Incrementing identifier ensures actuators can reject out of time frames
    comm->command.set_frame_id(comm->command.frame_id() + 1);
    if (comm->command.frame_id() > 65535) comm->command.set_frame_id(0);

    for (int i = 0; i < ROBIF2B_KINOVA_GEN3_NR_JOINTS; i++) {
        comm->command.mutable_actuators(i)->set_command_id(comm->command.frame_id());

        double pos = DEG_TO_RAD(comm->feedback.actuators(i).position());

        switch (*b->ctrl_mode) {
            case ROBIF2B_CTRL_MODE_POSITION:
                assert(b->jnt_pos_cmd);
                pos = b->jnt_pos_cmd[i];
            break;

            case ROBIF2B_CTRL_MODE_VELOCITY:
                assert(b->jnt_vel_cmd);
                pos += b->jnt_vel_cmd[i] * (*b->cycle_time);
            break;

            case ROBIF2B_CTRL_MODE_FORCE:
                assert(b->jnt_trq_cmd);
                comm->command.mutable_actuators(i)->set_torque_joint(b->jnt_trq_cmd[i]);
            break;

            case ROBIF2B_CTRL_MODE_CURRENT:
                assert(b->act_cur_cmd);
                comm->command.mutable_actuators(i)->set_current_motor(b->act_cur_cmd[i]);
            break;
        }

        // Unconditionally set the position command to avoid too large deviations
        comm->command.mutable_actuators(i)->set_position(RAD_TO_DEG(pos));
    }

    comm->feedback = comm->base_cyclic->Refresh(comm->command, 0);
    publish_measurement(b);

    b->ctrl_mode_prev = *b->ctrl_mode;

    *b->success = true;
}

