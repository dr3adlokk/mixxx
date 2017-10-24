#ifndef FLANGEREFFECT_H
#define FLANGEREFFECT_H

#include <QMap>

#include "effects/effectprocessor.h"
#include "engine/effects/engineeffect.h"
#include "engine/effects/engineeffectparameter.h"
#include "util/class.h"
#include "util/defs.h"
#include "util/sample.h"
#include "util/types.h"
#include "util/rampingvalue.h"

namespace {
constexpr double kMaxDelayMs = 13.0;
constexpr double kMinDelayMs = 0.22;
constexpr double kCenterDelayMs = (kMaxDelayMs - kMinDelayMs) / 2 + kMinDelayMs;
constexpr double kMaxLfoWidthMs = kMaxDelayMs - kMinDelayMs;
constexpr SINT kBufferLenth = static_cast<SINT>(ceil(kMaxDelayMs)) * 96; // for 96 kHz
constexpr double kMinLfoBeats = 1/4.0;
constexpr double kMaxLfoBeats = 32.0;
} // anonymous namespace

struct FlangerGroupState {
    FlangerGroupState()
            : delayPos(0),
              lfoFrames(0),
              previousPeriodFrames(-1),
              prev_regen(0),
              prev_mix(0),
              prev_width(0),
              prev_manual(kCenterDelayMs) {
        SampleUtil::applyGain(delayLeft, 0, kBufferLenth);
        SampleUtil::applyGain(delayRight, 0, kBufferLenth);
    }
    CSAMPLE delayRight[kBufferLenth];
    CSAMPLE delayLeft[kBufferLenth];
    unsigned int delayPos;
    unsigned int lfoFrames;
    double previousPeriodFrames;
    CSAMPLE_GAIN prev_regen;
    CSAMPLE_GAIN prev_mix;
    CSAMPLE_GAIN prev_width;
    CSAMPLE_GAIN prev_manual;
};

class FlangerEffect : public PerChannelEffectProcessor<FlangerGroupState> {
  public:
    FlangerEffect(EngineEffect* pEffect, const EffectManifest& manifest);
    virtual ~FlangerEffect();

    static QString getId();
    static EffectManifest getManifest();

    // See effectprocessor.h
    void processChannel(const ChannelHandle& handle,
                        FlangerGroupState* pState,
                        const CSAMPLE* pInput, CSAMPLE* pOutput,
                        const unsigned int numSamples,
                        const unsigned int sampleRate,
                        const EffectProcessor::EnableState enableState,
                        const GroupFeatureState& groupFeatures);

  private:
    QString debugString() const {
        return getId();
    }

    EngineEffectParameter* m_pSpeedParameter;
    EngineEffectParameter* m_pWidthParameter;
    EngineEffectParameter* m_pManualParameter;
    EngineEffectParameter* m_pRegenParameter;
    EngineEffectParameter* m_pMixParameter;
    EngineEffectParameter* m_pTripletParameter;

    DISALLOW_COPY_AND_ASSIGN(FlangerEffect);
};

#endif /* FLANGEREFFECT_H */
