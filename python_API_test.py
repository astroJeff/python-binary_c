import binary_c




def run_test_binary():
    m1 = 12.0
    m2 = 11.0
    eccentricity = 0.41
    metallicity = 0.02
    time = 33.895
    orbital_period = 4530.0

    sn_kick_magnitude_1 = 0.0
    sn_kick_theta_1 = 0.0
    sn_kick_phi_1 = 0.0
    sn_kick_magnitude_2 = 0.0
    sn_kick_theta_2 = 0.0
    sn_kick_phi_2 = 0.0

    output = binary_c.run_binary(m1, m2, orbital_period, eccentricity, metallicity, time,
                                 sn_kick_magnitude_1, sn_kick_theta_1, sn_kick_phi_1,
                                 sn_kick_magnitude_2, sn_kick_theta_2, sn_kick_phi_2)

    m1_out, m2_out, orbital_separation_out, eccentricity_out, system_velocity, L_x, \
            time_SN_1, time_SN_2, ktype_1, ktype_2, comenv_count, evol_hist = output

    evol_steps = evol_hist.split("\n")
    # evol_steps = evol_steps["JEFF" in evol_steps]

    for s in evol_steps:
        if s.find("JEFF") == 0:
            print s


    return output[:-1]



print run_test_binary()
