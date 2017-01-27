//#include "../../code/binary_c/src/API/binary_c_API.h"
#include "../../code/binary_c_newest/src/API/binary_c_API.h"

/* local function prototypes */
static double randd(double min, double max);
static void APIprintf(char * format,...);

static void set_srand(void);
static size_t getTotalSystemMemory(void);


int run_binary ( double m1, double m2, double orbital_period, double eccentricity,
                 double metallicity, double maxt,
                 double v_kick_1, double theta_kick_1, double phi_kick_1,
                 double v_kick_2, double theta_kick_2, double phi_kick_2,
                 double* m1_out, double* m2_out, double* orbital_separation_out,
                 double* eccentricity_out, double* system_velocity, double* L_x,
                 double* time_SN_1, double* time_SN_2, double* time_current,
                 int* ktype_1, int* ktype_2, int* comenv_count, int evol_flag,
                 char* evol_hist, int dco_flag);

int run_binaries ( double m1, double m2, double orbital_period, double eccentricity, double metallicity, double maxt );

/* C macros */
#define BINARY_C_APITEST_VERSION 0.1
#define APIprint(...) APIprintf(__VA_ARGS__);
#define NO_OUTPUT
