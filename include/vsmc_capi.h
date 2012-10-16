#ifndef VSMC_CAPI_H
#define VSMC_CAPI_H

#include <stddef.h>

/* Resample scheme */

#define VSMC_RESAMPLE_MULTINOMIAL           101
#define VSMC_RESAMPLE_RESIDUAL              102
#define VSMC_RESAMPLE_STRATIFIED            103
#define VSMC_RESAMPLE_SYSTEMATIC            104
#define VSMC_RESAMPLE_RESIDUAL_STRATIFIED   105
#define VSMC_RESAMPLE_RESIDUAL_SYSTEMATIC   106

/* Base class types */

#define VSMC_BASE_SEQUENTIAL 201
#define VSMC_BASE_CILK       202
#define VSMC_BASE_OMP        203
#define VSMC_BASE_TBB        204
#define VSMC_BASE_THREAD     205

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct
{
    void *sampler_ptr;
    int base_type;
} vsmcSamplerInfo;

typedef struct
{
    void *particle_ptr;
    int base_type;
} vsmcParticleInfo;

/* vsmc::Sampler */

vsmcSamplerInfo vsmc_sampler_new (size_t, unsigned, int, double, int);
void vsmc_sampler_delete                 (vsmcSamplerInfo);
size_t vsmc_sampler_size                 (vsmcSamplerInfo);
unsigned vsmc_sampler_iter_size          (vsmcSamplerInfo);
void vsmc_sampler_resample_scheme        (vsmcSamplerInfo, int);
double vsmc_sampler_resample_threshold   (vsmcSamplerInfo, double);
void vsmc_sampler_read_ess_history       (vsmcSamplerInfo, double *);
void vsmc_sampler_read_resampled_history (vsmcSamplerInfo, int *);
void vsmc_sampler_initialize             (vsmcSamplerInfo, void *);
void vsmc_sampler_iterate                (vsmcSamplerInfo, unsigned);

/* vsmc::Particle */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // VSMC_CAPI_H
