return block 
{
    name = "ubx_kinova_gen3",
    license = "LGPL3",
    meta_data = "Kinova Gen3 driver block",

    port_cache = true,

    configurations = {
        { name = "ctrl_mode", type_name = "int", min = 0, max = 1, doc = "initial ctrl_mode: 0: pos (def), 1: vel, 2: eff, 3: cur" },
        { name = "ip_address", type_name = "char" },
        { name = "port", type_name = "int" },
        { name = "port_real_time", type_name = "int" },
        { name = "user", type_name = "char" },
        { name = "password", type_name = "char" },
        { name = "session_timeout", type_name = "int", doc="[ms]" },
        { name = "connection_timeout", type_name = "int", doc="[ms]" },
    },

    ports = {
        { name = "ctrl_mode", in_type_name = "int", doc="port to switch control modes at runtime" },
        { name = "pos_msr", out_type_name = "double", out_data_len = 7, doc = "Measured joint position [rad]" },
        { name = "pos_cmd", in_type_name  = "double", in_data_len  = 7, doc = "Commanded joint position [rad]" },
        { name = "vel_msr", out_type_name = "double", out_data_len = 7, doc = "Measured joint velocity [rad/s]" },
        { name = "vel_cmd", in_type_name  = "double", in_data_len  = 7, doc = "Commanded joint velocity [rad/s]" },
        { name = "eff_msr", out_type_name = "double", out_data_len = 7, doc = "Measured joint effort [Nm]" },
        { name = "eff_cmd", in_type_name  = "double", in_data_len  = 7, doc = "Commanded joint effort [Nm]" },
        { name = "cur_msr", out_type_name = "double", out_data_len = 7, doc = "Measured joint current [A]" },
        { name = "cur_cmd", in_type_name  = "double", in_data_len  = 7, doc = "Commanded joint current [A]" },
    },

    operations = { start=true, stop=true, step=true }
}
