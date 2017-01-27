import binary_c




def run_test_binary():
    m1 = 15.0
    m2 = 14.0
    eccentricity = 0.41
    metallicity = 0.02
    time = 33.895
    orbital_period = 4530.0

    sn_kick_magnitude_1 = 150.0
    sn_kick_theta_1 = 0.0
    sn_kick_phi_1 = 0.0
    sn_kick_magnitude_2 = 100.0
    sn_kick_theta_2 = 0.0
    sn_kick_phi_2 = 0.0

    evol_flag = 0
    dco_flag = 1

    output = binary_c.run_binary(m1, m2, orbital_period, eccentricity, metallicity, time,
                                 sn_kick_magnitude_1, sn_kick_theta_1, sn_kick_phi_1,
                                 sn_kick_magnitude_2, sn_kick_theta_2, sn_kick_phi_2,
                                 evol_flag, dco_flag)

    m1_out, m2_out, orbital_separation_out, eccentricity_out, system_velocity, L_x, \
            time_SN_1, time_SN_2, time_current, ktype_1, ktype_2, comenv_count, evol_hist = output

    # print evol_hist

    evol_steps = evol_hist.split("\n")
    # evol_steps = evol_steps["JEFF" in evol_steps]

    n_comenv = "0"
    evol_line = ""

    for s in evol_steps:
        if s.find("JEFF") == 0:
            print s
            data = s.split(" ")
            k1_before, k2_before, k1_after, k2_after = data[2], data[3], data[15], data[16]
            # print s
            if n_comenv != data[28]:
                evol_line = evol_line + "(" + str(k1_before) + "-" + str(k2_before) + ":CE:" + str(k1_after) + "-" + str(k2_after) +  ")"
                n_comenv = data[28]
            else:
                evol_line = evol_line + "(" + str(k1_before) + "-" + str(k2_before) + ":" + str(k1_after) + "-" + str(k2_after) +  ")"

    print evol_line

    return output[:-1]



print run_test_binary()
