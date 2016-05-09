#include <avango/sound/Init.h>

#include <avango/Logger.h>

#include <avango/sound/SampleBuffer.h>
#include <avango/sound/SoundSource.h>
#include <avango/sound/SoundRenderer.h>
#include <avango/sound/SoundTraverser.h>

namespace
{
  av::Logger& logger(av::getLogger("av::sound::Init"));
}

AV_TYPED_DEFINE_ABSTRACT(av::sound::Init);

/* static */ void
av::sound::Init::initClass()
{
  if (!isTypeInitialized())
  {
    av::sound::SampleBuffer::initClass();
    av::sound::SoundSource::initClass();
    av::sound::SoundRenderer::initClass();
    av::sound::SoundTraverser::initClass();

    AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::sound::Init", true);
  }
}
