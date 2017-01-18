// #include "../../code/binary_c/src/API/binary_c_API.h"
#include "../../code/binary_c_newest/src/API/binary_c_API.h"
#include "binary_c_api.h"
#include <time.h>
#include <sys/timeb.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/*
 * apitest
 *
 * Short test programme to throw random binary systems at binary_c's
 * library via its API.
 *
 * Note that it looks more complicated than it is because I have included
 * code to capture binary_c's stdout stream and output it here.
 *
 * This code sends output to stderr : you should use apitest.sh to run it
 * and hence force output to your terminal's stdout.
 *
 * Output lines:
 *
 * APITEST ....  is information about what this code is doing.
 * STATUS  ....  is information about the binary system.
 * BINARY_C .... is output from binary_c (see iterate_logging.c etc.)
 *               which would have gone to stdout
 *
 * If you define the NO_OUTPUT macro, there will be no output except
 * the memory allocation and test system information. This is useful for speed tests,
 * but note that you may end up in a race condition where the pipe which replaces
 * stdout's buffer fills and hence the code stops.
 *
 * Note:
 * I have tested this with gcc 4.7.2 (Ubuntu 12.10) only.
 */


// #define _CAPTURE
#ifdef _CAPTURE
static void show_stdout(void);
static void capture_stdout(void);
#endif


/* global variables */
int out_pipe[2];
int stdoutwas;

// int main (int argc, char** argv)
// {
//
//   double m1, m2, orbital_period, eccentricity, metallicity, maxt;
//
//   return run_binary( m1, m2, orbital_period, eccentricity, metallicity, maxt );
// }

int main(){

    int out;

    out = run_binaries (2.0, 1.0, 100.0, 0.5, 10.0, 10.0 );

    double m1;
    double m2;
    double orbital_period;
    double eccentricity;
    double metallicity;
    double maxt;
    double v_kick_1;
    double theta_kick_1;
    double phi_kick_1;
    double v_kick_2;
    double theta_kick_2;
    double phi_kick_2;
    double m1_out;
    double m2_out;
    double orbital_separation_out;
    double eccentricity_out;
    double system_velocity;
    double L_x;
    double time_SN_1;
    double time_SN_2;
    int ktype_1;
    int ktype_2;
    int comenv_count;
    int evol_flag = 0;

    out = run_binary (m1, m2, orbital_period,  eccentricity, metallicity,  maxt,
                      v_kick_1, theta_kick_1, phi_kick_1,
                      v_kick_2, theta_kick_2, phi_kick_2,
                      &m1_out, &m2_out, &orbital_separation_out,
                      &eccentricity_out, &system_velocity, &L_x,
                      &time_SN_1, &time_SN_2, &ktype_1, &ktype_2,
                      &comenv_count, evol_flag);


}


int run_binary ( double m1, double m2, double orbital_period, double eccentricity, double metallicity, double maxt,
                 double v_kick_1, double theta_kick_1, double phi_kick_1,
                 double v_kick_2, double theta_kick_2, double phi_kick_2,
                 double* m1_out, double* m2_out, double* orbital_separation_out,
                 double* eccentricity_out, double* system_velocity, double* L_x,
                 double* time_SN_1, double* time_SN_2, int* ktype_1, int* ktype_2,
                 int* comenv_count, int evol_flag)
{

    const long int N=100; /* number of systems */
    // double maxt=15000.0; /* maximum time */
    double dt=10.0; /* timestep */
    double t=0.0; /* time */
    int i; /* counter */

    /* set random seed */
    set_srand();

    /* memory for N binary systems */
    struct libbinary_c_stardata_t *stardata;
    struct libbinary_c_store_t * store = NULL;
    char * argset1 = malloc(sizeof(char)*2048);
    char * argset2 = malloc(sizeof(char)*2048);
    char * argstring = malloc(sizeof(char)*4096);

    // double m1 = exp(randd(log(0.1),log(100.0)));
    sprintf(argset1,
            "binary_c M_1 %g M_2 %g eccentricity %g metallicity %g max_evolution_time %g idum -10 orbital_period %g monte_carlo_kicks 0",
            m1, // m2
            m2, // 0.1 < m2 < m1
            eccentricity, // ecc
            metallicity, // Z
            maxt, // max time (Myr)
            orbital_period
        );
    sprintf(argset2,
            " sn_kick_magnitude_1 %g sn_kick_theta_1 %g sn_kick_phi_1 %g sn_kick_magnitude_2 %g sn_kick_theta_2 %g sn_kick_phi_2 %g",
            v_kick_1,
            theta_kick_1,
            phi_kick_1,
            v_kick_2,
            theta_kick_2,
            phi_kick_2
          );

    /* combine two strings */
    snprintf(argstring, sizeof(char)*4096, "%s %s", argset1, argset2);
    // argstring = strcat(kickstring)


    // printf("argstring : %s\n",argstring);
    //fflush(stdout);

    stardata = NULL;
    binary_c_new_system(&stardata,
                        NULL,
                        NULL,
                        &store,
                        &argstring,
                        -1);


    /* log to /dev/null to ignore, /dev/stdout to view on screen */
    strcpy(stardata->preferences->log_filename,"/dev/null");
    //strcpy(stardata[i]->preferences->log_filename,"/dev/stdout");

    /* API log for debugging, usually /dev/null to disable */
    strcpy(stardata->preferences->api_log_filename_prefix,"/dev/null");
    stardata->preferences->internal_buffering = 2;
    stardata->preferences->internal_buffering_compression = 0;
    stardata->preferences->batchmode = BATCHMODE_LIBRARY;

    free(argstring);


    /* Stellar evolution time loop */
    while(t<maxt+TINY)
    {
       APIprint("At time t=%g evolve for %g\n",t,dt);
       APIprint("Call API to Evolve system %d from t=%g to %g\n",1,t,t+dt);

       /* evolve binary_c for time dt for system i */
       binary_c_evolve_for_dt(stardata,dt);

       APIprint("API evolved (stardata time now %g)\n",stardata->model.time);
  #ifndef NO_OUTPUT
       fprintf(stderr,"STATUS  : System %d : t=%g M1=%g M2=%g a=%g\n",
  	           i,
  	           stardata->model.time,
  	           stardata->star[0].mass,
  	           stardata->star[1].mass,
  	           stardata->common.separation);
  #endif
	     t += dt; // update the time
    }

    /* Save binary's history to the char* buffer */
    if (evol_flag == 1)
    {
       char * buffer = NULL;
       int nbytes = 0;
       binary_c_buffer_info(stardata,&buffer,&nbytes);
       binary_c_buffer_empty_buffer(stardata);
    }

    printf(buffer);

    *m1_out = stardata->star[0].mass;
    *m2_out = stardata->star[1].mass;
    *orbital_separation_out = stardata->common.separation;
    *eccentricity_out = stardata->common.eccentricity;
    *system_velocity = stardata->common.system_velocity;
    *L_x = MAX(stardata->star[0].Xray_luminosity, stardata->star[1].Xray_luminosity);
    *time_SN_1 = stardata->star[0].time_sn;
    *time_SN_2 = stardata->star[1].time_sn;
    *ktype_1 = stardata->star[0].stellar_type;
    *ktype_2 = stardata->star[1].stellar_type;
    *comenv_count = stardata->model.comenv_count;

    binary_c_free_memory(&stardata,TRUE,TRUE,FALSE);
    binary_c_free_store_contents(store);
    APIprint("API test finished\n");

    return 0;
}



int run_binaries ( double m1, double m2, double orbital_period, double eccentricity, double metallicity, double maxt )
{
    printf("binary_c apitest version %f (binary_c version %s)\n",
	       BINARY_C_APITEST_VERSION,BINARY_C_VERSION);

    const long int N=100; /* number of systems */
    // double maxt=15000.0; /* maximum time */
    double dt=10.0; /* timestep */
    double t=0.0; /* time */
    int i; /* counter */

    /* set random seed */
    set_srand();

    /* memory for N binary systems */
    struct libbinary_c_stardata_t *stardata[N];
    struct libbinary_c_store_t * store = NULL;
    char * argstring = malloc(sizeof(char)*4096);
    for(i=0;i<N;i++)
    {

        // double m1 = exp(randd(log(0.1),log(100.0)));
        sprintf(argstring,
                "binary_c M_1 %g M_2 %g eccentricity %g metallicity %g max_evolution_time %g idum -10 orbital_period %g",
                m1, // m2
                m2, // 0.1 < m2 < m1
                eccentricity, // ecc
                metallicity, // Z
                maxt, // max time (Myr)
                orbital_period
            );
        //printf("argstring : %s\n",argstring);
        //fflush(stdout);

        stardata[i] = NULL;
        binary_c_new_system(stardata + i,
                            NULL,
                            NULL,
                            &store,
                            &argstring,
                            -1);

        /* log to /dev/null to ignore, /dev/stdout to view on screen */
        strcpy(stardata[i]->preferences->log_filename,"/dev/null");
        //strcpy(stardata[i]->preferences->log_filename,"/dev/stdout");

        /* API log for debugging, usually /dev/null to disable */
        strcpy(stardata[i]->preferences->api_log_filename_prefix,"/dev/null");
        stardata[i]->preferences->internal_buffering = 2;
        stardata[i]->preferences->internal_buffering_compression = 0;
        stardata[i]->preferences->batchmode = BATCHMODE_LIBRARY;
    }
    free(argstring);

    /* find available system RAM (bytes) */
    long int RAM_available=(long int)getTotalSystemMemory();

    printf("Memory : stardata (x%ld) %8.5f MB prefs %8.5f MB (could alloc %ld stars in %g MB of RAM)\n",
	   N,
	   N*sizeof(struct stardata_t)/(1024.0*1024.0),
	   sizeof(struct preferences_t)/(1024.0*1024.0),
	   RAM_available/sizeof(struct stardata_t),
	   RAM_available/(1024.0*1024.0)
	);

    /* Stellar evolution time loop */
    while(t<maxt+TINY)
    {
	APIprint("At time t=%g evolve for %g\n",t,dt);
	for(i=0;i<N;i++)
	{
	    APIprint("Call API to Evolve system %d from t=%g to %g\n",i,t,t+dt);

	    /* evolve binary_c for time dt for system i */
	    binary_c_evolve_for_dt(stardata[i],dt);

            APIprint("API evolved (stardata time now %g)\n",stardata[i]->model.time);
#ifndef NO_OUTPUT
	    fprintf(stderr,"STATUS  : System %d : t=%g M1=%g M2=%g a=%g\n",
		    i,
		    stardata[i]->model.time,
		    stardata[i]->star[1].mass,
		    stardata[i]->star[2].mass,
		    stardata[i]->common.separation);
#endif
	}
	t += dt; // update the time
    }

    /* free memory */
    for(i=0;i<N;i++)
    {
        binary_c_free_memory(stardata+i,TRUE,TRUE,FALSE);
    }
    binary_c_free_store_contents(store);
    APIprint("API test finished\n");

    return 0;
}

/************************************************************/
#ifdef _CAPTURE

static void capture_stdout(void)
{
    /* capture stdout to a pipe */
    fflush(stdout);
    setvbuf(stdout,NULL,_IONBF,0); // remove stdout buffering
    setvbuf(stderr,NULL,_IONBF,0); // remove stdout buffering

    /* save for later reset */
    stdoutwas = dup(STDOUT_FILENO);

    if(pipe(out_pipe)!=0)
    {
	fprintf(stderr,"pipe creation failure\n");
	Exit_binary_c_no_stardata(PIPE_FAILURE,"pipe creation failed");
    }

    /* non-blocking stdout */
    long flags = fcntl(out_pipe[0], F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(out_pipe[0], F_SETFL, flags);

    /* never write to the new pipe */
    dup2(out_pipe[1],STDOUT_FILENO);
    close(out_pipe[1]);
}


static void show_stdout(void)
{
    APIprint("show_stdout:\n");

    /* show what would have been binary_c's stdout */
    char buffer;
    fflush(stderr);
    fflush(stdout);
#ifndef NO_OUTPUT
    int stat=1;
#endif
    while(read(out_pipe[0],&buffer,1)>0)
    {
#ifndef NO_OUTPUT
	if(stat==1) fprintf(stderr,"BINARY_C: ");
	fprintf(stderr,"%c",buffer);
	stat = buffer == '\n' ? 1 : 0;
#endif
    }
}
#endif // _CAPTURE

static double randd(double min, double max)
{
    // random number between min and max
    return min + (max-min) * ((double)rand()/(double)RAND_MAX);
}

static void APIprintf(char * format,...)
{
    /* API print function */
#ifndef NO_OUTPUT
    va_list args;
    va_start(args,format);

    /* s contains the message */
    static char s[10000];
    vsnprintf(s,10000,format,args);
    chomp(s); // from binary_c
    fprintf(stderr,"APITEST : %s\n",s);
    fflush(stderr);
    va_end(args);
#endif
}

void set_srand(void)
{
    /* randomness based on the current time */
    struct timeb tp;
    long int tt;
    ftime(&tp);
    tt=1000*tp.time+tp.millitm;
    srand(-((long int)(tt))%(DAY_LENGTH_IN_SECONDS)-1);
}

static size_t getTotalSystemMemory(void)
{
    /* use sysconf to get available memory */
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size;
}
