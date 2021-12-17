#include "hl2sdk.h"
#include <stdexcept>

/* Fix undefined references */
const QAngle vec3_angle;

void MatrixGetColumn( const matrix3x4_t &in, int column, Vector &out ) UNDEF_CALL
void MatrixSetColumn( const Vector &in, int column, matrix3x4_t &out ) UNDEF_CALL
void SetIdentityMatrix(matrix3x4_t &mat) UNDEF_CALL
void DefaultRenderBoundsWorldspace(IClientRenderable *pRenderable, Vector &absMins, Vector &absMaxs) UNDEF_CALL

bool CParticleCollection::IsTranslucent() const UNDEF_CALL
const char *CParticleCollection::GetName() const UNDEF_CALL
CParticleCollection::~CParticleCollection() UNDEF_CALL
void CParticleCollection::operator delete(void *pData) UNDEF_CALL

CNewParticleEffect::~CNewParticleEffect() UNDEF_CALL
void CNewParticleEffect::Update(float flTimeDelta) UNDEF_CALL
void CNewParticleEffect::NotifyRemove() UNDEF_CALL
bool CNewParticleEffect::IsTwoPass() UNDEF_CALL
bool CNewParticleEffect::UsesPowerOfTwoFrameBufferTexture() UNDEF_CALL
bool CNewParticleEffect::UsesFullFrameBufferTexture() UNDEF_CALL
void CNewParticleEffect::GetRenderBounds(Vector &mins, Vector &maxs) UNDEF_CALL
int CNewParticleEffect::DrawModel(int flags) UNDEF_CALL
void CNewParticleEffect::SetParticleCullRadius(float radius) UNDEF_CALL
