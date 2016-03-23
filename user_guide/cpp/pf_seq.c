#include <vsmc/vsmc.h>
#include <math.h>
#include <stdio.h>

static const int N = 1000; // Number of particles
static const int n = 100;  // Number of data points
static const int PosX = 0;
static const int PosY = 1;
static const int VelX = 2;
static const int VelY = 3;

static double pf_obs_x[n];
static double pf_obs_y[n];

// Temporaries used by pf_init and pf_move
static double pf_pos_x[N];
static double pf_pos_y[N];
static double pf_vel_x[N];
static double pf_vel_y[N];
static double pf_weight[N];

static double pf_log_likelihood(int t, const vsmc_single_particle *sp)
{
    double llh_x = 10 * (sp->state[PosX] - pf_obs_x[t]);
    double llh_y = 10 * (sp->state[PosY] - pf_obs_y[t]);
    llh_x = log(1 + llh_x * llh_x / 10);
    llh_y = log(1 + llh_y * llh_y / 10);

    return -0.5 * (10 + 1) * (llh_x + llh_y);
}

static void pf_read_data(const char *param)
{
    if (!param)
        return;

    FILE *data = fopen(param, "r");
    for (int i = 0; i < n; ++i) {
        fscanf(data, "%lg ", pf_obs_x + i);
        fscanf(data, "%lg ", pf_obs_y + i);
    }
    fclose(data);
}

static int pf_init(vsmc_particle particle, void *param)
{
    pf_read_data((const char *) param);

    vsmc_rng rng = vsmc_particle_rng(particle, 0);
    const int size = vsmc_particle_size(particle);
    const double sd_pos = 2;
    const double sd_vel = 1;

    vsmc_normal_distribution(rng, size, pf_pos_x, 0, sd_pos);
    vsmc_normal_distribution(rng, size, pf_pos_y, 0, sd_pos);
    vsmc_normal_distribution(rng, size, pf_vel_x, 0, sd_vel);
    vsmc_normal_distribution(rng, size, pf_vel_y, 0, sd_vel);
    for (int i = 0; i < size; ++i) {
        vsmc_single_particle sp = vsmc_particle_sp(particle, i);
        sp.state[PosX] = pf_pos_x[i];
        sp.state[PosY] = pf_pos_y[i];
        sp.state[VelX] = pf_vel_x[i];
        sp.state[VelY] = pf_vel_y[i];
        pf_weight[i] = pf_log_likelihood(0, &sp);
    }
    vsmc_weight_set_log(vsmc_particle_weight(particle), pf_weight, 1);

    return 0;
}

static int pf_move(int t, vsmc_particle particle)
{
    vsmc_rng rng = vsmc_particle_rng(particle, 0);
    const int size = vsmc_particle_size(particle);
    const double sd_pos = sqrt(0.02);
    const double sd_vel = sqrt(0.001);

    vsmc_normal_distribution(rng, size, pf_pos_x, 0, sd_pos);
    vsmc_normal_distribution(rng, size, pf_pos_y, 0, sd_pos);
    vsmc_normal_distribution(rng, size, pf_vel_x, 0, sd_vel);
    vsmc_normal_distribution(rng, size, pf_vel_y, 0, sd_vel);
    for (int i = 0; i < size; ++i) {
        vsmc_single_particle sp = vsmc_particle_sp(particle, i);
        sp.state[PosX] += pf_pos_x[i] + 0.1 * sp.state[VelX];
        sp.state[PosY] += pf_pos_y[i] + 0.1 * sp.state[VelY];
        sp.state[VelX] += pf_vel_x[i];
        sp.state[VelY] += pf_vel_y[i];
        pf_weight[i] = pf_log_likelihood(t, &sp);
    }
    vsmc_weight_add_log(vsmc_particle_weight(particle), pf_weight, 1);

    return 0;
}

static void pf_eval(int t, int dim, vsmc_particle particle, double *r)
{
    const int size = vsmc_particle_size(particle);
    for (int i = 0; i < size; ++i) {
        vsmc_single_particle sp = vsmc_particle_sp(particle, i);
        *r++ = sp.state[PosX];
        *r++ = sp.state[PosY];
    }
}

int main()
{
    vsmc_sampler sampler;
    vsmc_sampler_malloc(&sampler, N, 4, vSMCMultinomial, 0.5);
    vsmc_sampler_init(sampler, pf_init);
    vsmc_sampler_move(sampler, pf_move, 0);
    vsmc_sampler_set_monitor(sampler, "pos", 2, pf_eval, 0, vSMCMonitorMCMC);

    vsmc_sampler_initialize(sampler, (void *) "pf.data");
    vsmc_sampler_iterate(sampler, n - 1);

    vsmc_sampler_save_f(sampler, "pf.out");

    return 0;
}
