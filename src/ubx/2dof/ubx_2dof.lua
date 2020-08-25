return block 
{
    name = "ubx_2dof",
    license = "LGPL3",
    meta_data = "2-DoF robot's driver block",

    port_cache = true,

    ports = {
        { name = "pos_msr", out_type_name = "double", out_data_len = 2, doc = "Measured joint position [rad]" },
        { name = "vel_msr", out_type_name = "double", out_data_len = 2, doc = "Measured joint velocity [rad/s]" },
        { name = "vel_cmd", in_type_name  = "double", in_data_len  = 2, doc = "Commanded joint velocity [rad/s]" },
    },

    operations = { step=true }
}
