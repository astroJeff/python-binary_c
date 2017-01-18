import binary_c




def run_test_binary():
    m1 = 12.0
    m2 = 9.0
    eccentricity = 0.41
    metallicity = 0.02
    time = 33.895
    orbital_period = 13000.0


    sn_kick_magnitude_1 = 0.0
    sn_kick_theta_1 = 0.0
    sn_kick_phi_1 = 0.0
    sn_kick_magnitude_2 = 0.0
    sn_kick_theta_2 = 0.0
    sn_kick_phi_2 = 0.0

    output = binary_c.run_binary(m1, m2, orbital_period, eccentricity, metallicity, time,
                                         sn_kick_magnitude_1, sn_kick_theta_1, sn_kick_phi_1,
                                         sn_kick_magnitude_2, sn_kick_theta_2, sn_kick_phi_2)

    return output



print run_test_binary()
