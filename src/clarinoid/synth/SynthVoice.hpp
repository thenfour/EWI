
#pragma once

#ifdef CLARINOID_MODULE_TEST
#error not for x86 tests
#endif

#include <cfloat>

#include <clarinoid/basic/Basic.hpp>
#include <clarinoid/loopstation/LoopstationMemory.hpp>

#include "FilterNode.hpp"
#include "PannerNode.hpp"
#include "MultiMixer.hpp"
#include "ModulationMatrixNode.hpp"
#include "Patch.hpp"
#include "polyBlepOscillator.hpp"

namespace clarinoid
{

namespace CCSynthGraph
{
/*
https://www.pjrc.com/teensy/gui/index.html
// this is the after-oscillator processing.

*/
// GUItool: begin automatically generated code
AudioAmplifier delayFeedbackAmpLeft;  // xy=535,170
AudioEffectDelay delayRight;          // xy=536,478
AudioEffectDelay delayLeft;           // xy=537,261
AudioAmplifier delayFeedbackAmpRight; // xy=537,390
AudioAmplifier delayWetAmpLeft;       // xy=781,234
AudioAmplifier delayWetAmpRight;      // xy=802,469
AudioMixer4 verbInputMixer;           // xy=920,561
AudioSynthWaveformSine metronomeOsc;  // xy=956,990
AudioEffectFreeverbStereo verb;       // xy=1070,567
AudioEffectEnvelope metronomeEnv;     // xy=1157,992
AudioAmplifier verbWetAmpLeft;        // xy=1229,548
AudioAmplifier verbWetAmpRight;       // xy=1236,586
AudioMixer4 postMixerLeft;            // xy=1411,775
AudioMixer4 postMixerRight;           // xy=1413,858
AudioAmplifier ampLeft;               // xy=1573,776
AudioAmplifier ampRight;              // xy=1576,857
AudioOutputI2S i2s1;                  // xy=1758,809
AudioAnalyzePeak peak1;               // xy=1801,588
AudioConnection patchCord1(delayWetAmpLeft, 0, postMixerLeft, 1);
AudioConnection patchCord2(delayWetAmpRight, 0, postMixerRight, 1);
AudioConnection patchCord3(verbInputMixer, verb);
AudioConnection patchCord4(metronomeOsc, metronomeEnv);
AudioConnection patchCord5(verb, 0, verbWetAmpLeft, 0);
AudioConnection patchCord6(verb, 1, verbWetAmpRight, 0);
AudioConnection patchCord7(metronomeEnv, 0, postMixerRight, 3);
AudioConnection patchCord8(metronomeEnv, 0, postMixerLeft, 3);
AudioConnection patchCord9(verbWetAmpLeft, 0, postMixerLeft, 2);
AudioConnection patchCord10(verbWetAmpRight, 0, postMixerRight, 2);
AudioConnection patchCord11(postMixerLeft, ampLeft);
AudioConnection patchCord12(postMixerRight, ampRight);
AudioConnection patchCord13(ampLeft, peak1);
AudioConnection patchCord14(ampLeft, 0, i2s1, 1);
AudioConnection patchCord15(ampRight, 0, i2s1, 0);
// GUItool: end automatically generated code

// insert the delay filters.
// ...[delayLeft]---->[delayFilterLeft]--------------->[delayFeedbackAmpLeft]...
//                                     \-------------->[delayWetAmpLeft]...
::clarinoid::FilterNode delayFilterLeft;
AudioConnection mPatchDelayToFilterLeft = {delayLeft, 0, delayFilterLeft, 0};
AudioConnection mPatchDelayFilterToFeedbackAmpLeft = {delayFilterLeft, 0, delayFeedbackAmpLeft, 0};

::clarinoid::FilterNode delayFilterRight;
AudioConnection mPatchDelayToFilterRight = {delayRight, 0, delayFilterRight, 0};
AudioConnection mPatchDelayFilterToFeedbackAmpRight = {delayFilterRight, 0, delayFeedbackAmpRight, 0};

// delay output connection
AudioConnection mPatchDelayFilterToAmpLeft = {delayFilterLeft, 0, delayWetAmpLeft, 0};
AudioConnection mPatchDelayFilterToAmpRight = {delayFilterRight, 0, delayWetAmpRight, 0};

// voice mixer & dry output connection
::clarinoid::MultiMixerNode<MAX_SYNTH_VOICES> voiceMixerDryLeft;  // all voices input here.
::clarinoid::MultiMixerNode<MAX_SYNTH_VOICES> voiceMixerDryRight; // all voices input here.
AudioConnection patchVoicesDryToOutpLeft = {voiceMixerDryLeft, 0, postMixerLeft, 0};
AudioConnection patchVoicesDryToOutpRight = {voiceMixerDryRight, 0, postMixerRight, 0};

// delay input mix
::clarinoid::MultiMixerNode<MAX_SYNTH_VOICES + 1> delayInputMixerLeft;  // +1 to account for the delay feedback signal
::clarinoid::MultiMixerNode<MAX_SYNTH_VOICES + 1> delayInputMixerRight; // +1 to account for the delay feedback signal
AudioConnection patchDelayInputMixToDelayL = {delayInputMixerLeft, 0, delayLeft, 0};
AudioConnection patchDelayInputMixToDelayR = {delayInputMixerRight, 0, delayRight, 0};

// delay fb
AudioConnection patchDelayFBBackToInputLeft = {delayFeedbackAmpLeft, 0, delayInputMixerLeft, MAX_SYNTH_VOICES};
AudioConnection patchDelayFBBackToInputRight = {delayFeedbackAmpRight, 0, delayInputMixerRight, MAX_SYNTH_VOICES};

// verb input mix
::clarinoid::MultiMixerNode<MAX_SYNTH_VOICES + 1> verbInputMixerLeft;  // all voices input here + 1 for delay line
::clarinoid::MultiMixerNode<MAX_SYNTH_VOICES + 1> verbInputMixerRight; // all voices input here + 1 for delay line
AudioConnection patchVerbInputToVerbL = {verbInputMixerLeft, 0, verbInputMixer, 0};
AudioConnection patchVerbInputToVerbR = {verbInputMixerRight, 0, verbInputMixer, 1};

// delay verb
AudioConnection patchDelayVerbInputLeft = {delayFilterLeft, 0, verbInputMixerLeft, MAX_SYNTH_VOICES};
AudioConnection patchDelayVerbInputRight = {delayFilterRight, 0, verbInputMixerRight, MAX_SYNTH_VOICES};

} // namespace CCSynthGraph

struct Voice
{
    //
    // [mOsc osc1] -->
    // [mOsc osc2] --> [mOscMixer] --> [mFilter] -> [mPannerSplitter]
    // [mOsc osc3] -->
    //
    AudioBandlimitedOsci mOsc;

    // Modulation sources
    VoiceModulationMatrixNode mModMatrix;
    AudioEffectEnvelope mEnv1;
    AudioEffectEnvelope mEnv2;
    AudioSynthWaveformDc mEnv1PeakDC;
    AudioSynthWaveformDc mEnv2PeakDC;
    CCPatch mPatchDCToEnv1 = {mEnv1PeakDC, 0, mEnv1, 0};
    CCPatch mPatchDCToEnv2 = {mEnv2PeakDC, 0, mEnv2, 0};
    AudioSynthWaveform mLfo1;
    AudioSynthWaveform mLfo2;
    AudioSynthWaveformDc mBreathModSource;
    AudioSynthWaveformDc mPitchBendModSource;

    // have to track these because they're private members of AudioWaveform.
    short mLfo1Waveshape = 0xff; // invalid so 1st run will always set the shape.
    short mLfo2Waveshape = 0xff; // invalid so 1st run will always set the shape.

    // patch modulation sources in
    CCPatch mPatchEnv1ToMod = {mEnv1, 0, mModMatrix, (uint8_t)ModulationSourceToIndex(ModulationSource::ENV1)};
    CCPatch mPatchEnv2ToMod = {mEnv2, 0, mModMatrix, (uint8_t)ModulationSourceToIndex(ModulationSource::ENV2)};
    CCPatch mPatchLfo1ToMod = {mLfo1, 0, mModMatrix, (uint8_t)ModulationSourceToIndex(ModulationSource::LFO1)};
    CCPatch mPatchLfo2ToMod = {mLfo2, 0, mModMatrix, (uint8_t)ModulationSourceToIndex(ModulationSource::LFO2)};

    CCPatch mPatchBreathToMod = {mBreathModSource,
                                 0,
                                 mModMatrix,
                                 (uint8_t)ModulationSourceToIndex(ModulationSource::Breath)};
    CCPatch mPatchPitchStripToMod = {mPitchBendModSource,
                                     0,
                                     mModMatrix,
                                     (uint8_t)ModulationSourceToIndex(ModulationSource::PitchStrip)};

    // Patch modulation destinations
    /*
Input 0: Frequency Modulation for Oscillator 1
Input 1: Pulse Width Modulation for Oscillator 1
Input 2: Frequency Modulation for Oscillator 2
Input 3: Pulse Width Modulation for Oscillator 2
Input 4: Frequency Modulation for Oscillator 3
Input 5: Pulse Width Modulation for Oscillator 3
    */
    CCPatch mPatchModToOsc1PWM = {mModMatrix,
                                  (uint8_t)ModulationDestinationToIndex(ModulationDestination::Osc1PulseWidth),
                                  mOsc,
                                  1};
    CCPatch mPatchModToOsc2PWM = {mModMatrix,
                                  (uint8_t)ModulationDestinationToIndex(ModulationDestination::Osc2PulseWidth),
                                  mOsc,
                                  3};
    CCPatch mPatchModToOsc3PWM = {mModMatrix,
                                  (uint8_t)ModulationDestinationToIndex(ModulationDestination::Osc3PulseWidth),
                                  mOsc,
                                  5};

    CCPatch mPatchModToOsc1Freq = {mModMatrix,
                                   (uint8_t)ModulationDestinationToIndex(ModulationDestination::Osc1Frequency),
                                   mOsc,
                                   0};
    CCPatch mPatchModToOsc2Freq = {mModMatrix,
                                   (uint8_t)ModulationDestinationToIndex(ModulationDestination::Osc2Frequency),
                                   mOsc,
                                   2};
    CCPatch mPatchModToOsc3Freq = {mModMatrix,
                                   (uint8_t)ModulationDestinationToIndex(ModulationDestination::Osc3Frequency),
                                   mOsc,
                                   4};

    // ...
    MultiMixerNode<3> mOscMixer;

    CCPatch mPatchOsc1ToMixer = {mOsc, 0, mOscMixer, 0};
    CCPatch mPatchOsc2ToMixer = {mOsc, 1, mOscMixer, 1};
    CCPatch mPatchOsc3ToMixer = {mOsc, 2, mOscMixer, 2};

    ::clarinoid::FilterNode mFilter;
    CCPatch mPatchMixToFilter = {mOscMixer, 0, mFilter, 0};

    ::clarinoid::GainAndPanSplitterNode<3> mPannerSplitter; // handles panning and send levels to delay/verb/dry
    CCPatch mPatchFilterToPannerSplitter = {mFilter, 0, mPannerSplitter, 0};

    CCPatch mPatchOutDryLeft;
    CCPatch mPatchOutDryRight;

    CCPatch mPatchOutDelayLeft;
    CCPatch mPatchOutDelayRight;

    CCPatch mPatchOutVerbLeft;
    CCPatch mPatchOutVerbRight;

    MusicalVoice mRunningVoice;
    SynthPreset *mPreset = nullptr;
    AppSettings *mAppSettings;
    bool mTouched = false;

    void EnsurePatchConnections(AppSettings *appSettings)
    {
        mAppSettings = appSettings;

        mEnv1PeakDC.amplitude(1.0f);
        mEnv2PeakDC.amplitude(1.0f);

        mPatchOsc1ToMixer.connect();
        mPatchOsc2ToMixer.connect();
        mPatchOsc3ToMixer.connect();
        mPatchMixToFilter.connect();
        mPatchFilterToPannerSplitter.connect();
        mPatchOutDryLeft.connect();
        mPatchOutDryRight.connect();
        mPatchOutDelayLeft.connect();
        mPatchOutDelayRight.connect();
        mPatchOutVerbLeft.connect();
        mPatchOutVerbRight.connect();

        mPatchEnv1ToMod.connect();
        mPatchEnv2ToMod.connect();
        mPatchLfo1ToMod.connect();
        mPatchLfo2ToMod.connect();

        mPatchBreathToMod.connect();
        mPatchPitchStripToMod.connect();

        mPatchModToOsc1PWM.connect();
        mPatchModToOsc2PWM.connect();
        mPatchModToOsc3PWM.connect();

        mPatchModToOsc1Freq.connect();
        mPatchModToOsc2Freq.connect();
        mPatchModToOsc3Freq.connect();

        mPatchDCToEnv1.connect();
        mPatchDCToEnv2.connect();
    }

    static float CalcFilterCutoffFreq(float breath01,
                                      float midiNote,
                                      float keyTrackingAmt,
                                      float freqMin,
                                      float freqMax)
    {
        // perform breath & key tracking for filter. we will basically multiply the
        // effects. velocity we will only track between notes from 7jam code: const
        // halfKeyScaleRangeSemis = 12 * 4; from 7jam code: let ks = 1.0 -
        // DF.remap(this.midiNote, 60.0 /* middle C */ -
        // halfKeyScaleRangeSemis, 60.0 + halfKeyScaleRangeSemis, ksAmt, -ksAmt); //
        // when vsAmt is 0, the range of vsAmt,-vsAmt is 0. hence making this 1.0-x
        float filterKS = map(midiNote, 20, 120, 0.0f, 1.0f); // map midi note to full ks effect
        filterKS = map(keyTrackingAmt, 0, 1.0f, 1.0,
                       filterKS); // map ks amt 0-1 to 1-fulleffect

        float filterP = filterKS * breath01;
        filterP = ClampInclusive(filterP, 0.0f, 1.0f);

        float filterFreq = map(filterP, 0.0f, 1.0f, freqMin, freqMax);
        return filterFreq;
    }

    void Update(const MusicalVoice &mv)
    {
        mPreset = &mAppSettings->FindSynthPreset(mv.mSynthPatch);
        mModMatrix.SetSynthPatch(mPreset);
        auto transition = CalculateTransitionEvents(mRunningVoice, mv);
        bool voiceOrPatchChanged =
            (mRunningVoice.mVoiceId != mv.mVoiceId) || (mRunningVoice.mSynthPatch != mv.mSynthPatch);
        if (voiceOrPatchChanged || transition.mNeedsNoteOff)
        {
            mOsc.removeNote();
        }
        if (voiceOrPatchChanged || transition.mNeedsNoteOn)
        {
            mOsc.addNote(); // this also engages portamento
        }

        if (!mv.IsPlaying() || mv.mIsNoteCurrentlyMuted)
        {
            mOsc.amplitude(1, 0.0);
            mOsc.amplitude(2, 0.0);
            mOsc.amplitude(3, 0.0);
            mRunningVoice = mv;
            return;
        }

        // configure envelopes (DADSR x 3)
        mEnv1.delay(mPreset->mEnv1.mDelayMS);
        mEnv1.attack(mPreset->mEnv1.mAttackMS);
        mEnv1.hold(0);
        mEnv1.decay(mPreset->mEnv1.mDecayMS);
        mEnv1.sustain(mPreset->mEnv1.mSustainLevel);
        mEnv1.release(mPreset->mEnv1.mReleaseMS);
        mEnv1.releaseNoteOn(0);

        mEnv2.delay(mPreset->mEnv2.mDelayMS);
        mEnv2.attack(mPreset->mEnv2.mAttackMS);
        mEnv2.hold(0);
        mEnv2.decay(mPreset->mEnv2.mDecayMS);
        mEnv2.sustain(mPreset->mEnv2.mSustainLevel);
        mEnv2.release(mPreset->mEnv2.mReleaseMS);
        mEnv2.releaseNoteOn(0);

        auto convertWaveType = [](OscWaveformShape s) {
            switch (s)
            {
            case OscWaveformShape::Pulse:
                return WAVEFORM_PULSE;
            case OscWaveformShape::SawSync:
                return WAVEFORM_SAWTOOTH;
            case OscWaveformShape::VarTriangle:
                return WAVEFORM_TRIANGLE;
            case OscWaveformShape::Sine:
                break;
            }
            return WAVEFORM_SINE;
        };

        short wantsWaveType1 = convertWaveType(mPreset->mLfo1Shape);
        short wantsWaveType2 = convertWaveType(mPreset->mLfo2Shape);
        if (mLfo1Waveshape != wantsWaveType1)
        {
            mLfo1.begin(wantsWaveType1);
            mLfo1.amplitude(1.0f);
        }
        if (mLfo2Waveshape != wantsWaveType2)
        {
            mLfo2.begin(wantsWaveType2);
            mLfo2.amplitude(1.0f);
        }

        mLfo1.frequency(mPreset->mLfo1Rate);
        mLfo2.frequency(mPreset->mLfo2Rate);

        mBreathModSource.amplitude(mv.mBreath01.GetFloatVal());
        mPitchBendModSource.amplitude(mv.mPitchBendN11.GetFloatVal());

        if (voiceOrPatchChanged || transition.mNeedsNoteOff)
        {
            mEnv1.noteOff();
            mEnv2.noteOff();
        }

        if (voiceOrPatchChanged || transition.mNeedsNoteOn)
        {
            mEnv1.noteOn();
            mEnv2.noteOn();
        }

        mOsc.amplitude(1, mPreset->mOsc1Gain);
        mOsc.amplitude(2, mPreset->mOsc2Gain);
        mOsc.amplitude(3, mPreset->mOsc3Gain);

        // update
        float midiNote =
            (float)mv.mMidiNote + mv.mPitchBendN11.GetFloatVal() * mAppSettings->mSynthSettings.mPitchBendRange;

        mOsc.portamentoTime(1, mPreset->mOsc1PortamentoTime);
        mOsc.portamentoTime(2, mPreset->mOsc2PortamentoTime);
        mOsc.portamentoTime(3, mPreset->mOsc3PortamentoTime);

        mOsc.waveform(1, mPreset->mOsc1Waveform);
        mOsc.waveform(2, mPreset->mOsc2Waveform);
        mOsc.waveform(3, mPreset->mOsc3Waveform);

        mOsc.pulseWidth(1, mPreset->mOsc1PulseWidth);
        mOsc.pulseWidth(2, mPreset->mOsc2PulseWidth);
        mOsc.pulseWidth(3, mPreset->mOsc3PulseWidth);

        // pwm amount is always 1 within the polyblep osc; actual PWM amount is controlled by modulation matrix.
        mOsc.pwmAmount(1, 1);
        mOsc.pwmAmount(2, 1);
        mOsc.pwmAmount(3, 1);

        mOsc.fmAmount(1, 1);
        mOsc.fmAmount(2, 1);
        mOsc.fmAmount(3, 1);

        auto calcFreq =
            [](float midiNote, float pitchFine, int pitchSemis, float detune, float freqMul, float freqOffset) {
                float ret = midiNote + pitchFine + pitchSemis + detune;
                ret = (MIDINoteToFreq(ret) * freqMul) + freqOffset;
                return Clamp(ret, 0.0f, 22050.0f);
            };

        mOsc.frequency(1,
                       calcFreq(midiNote,
                                mPreset->mOsc1PitchFine,
                                mPreset->mOsc1PitchSemis,
                                -mPreset->mDetune,
                                mPreset->mOsc1FreqMultiplier,
                                mPreset->mOsc1FreqOffset));
        mOsc.frequency(3,
                       calcFreq(midiNote,
                                mPreset->mOsc3PitchFine,
                                mPreset->mOsc3PitchSemis,
                                mPreset->mDetune,
                                mPreset->mOsc3FreqMultiplier,
                                mPreset->mOsc3FreqOffset));

        if (mPreset->mSync)
        {
            float freq = calcFreq(midiNote,
                                  mPreset->mOsc2PitchFine,
                                  mPreset->mOsc2PitchSemis,
                                  0,
                                  mPreset->mOsc2FreqMultiplier,
                                  mPreset->mOsc2FreqOffset);
            float freqSync =
                map(mv.mBreath01.GetFloatVal(), 0.0f, 1.0f, freq * mPreset->mSyncMultMin, freq * mPreset->mSyncMultMax);
            mOsc.frequency(2, freqSync);
        }
        else
        {
            mOsc.frequency(2,
                           calcFreq(midiNote,
                                    mPreset->mOsc2PitchFine,
                                    mPreset->mOsc2PitchSemis,
                                    0,
                                    mPreset->mOsc2FreqMultiplier,
                                    mPreset->mOsc2FreqOffset));
        }

        // perform breath & key tracking for filter. we will basically multiply the
        // effects.
        float filterFreq = CalcFilterCutoffFreq(mv.mBreath01.GetFloatVal(),
                                                midiNote,
                                                mPreset->mFilterKeytracking,
                                                mPreset->mFilterMinFreq,
                                                mPreset->mFilterMaxFreq);

        mFilter.SetParams(mPreset->mFilterType, filterFreq, mPreset->mFilterQ, mPreset->mFilterSaturation);
        mFilter.EnableDCFilter(mPreset->mDCFilterEnabled, mPreset->mDCFilterCutoff);

        mPannerSplitter.SetPanAndGain(0, 1.0f, mv.mPan + mPreset->mPan);
        mPannerSplitter.SetPanAndGain(1, mPreset->mDelaySend, mv.mPan + mPreset->mPan);
        mPannerSplitter.SetPanAndGain(2, mPreset->mVerbSend, mv.mPan + mPreset->mPan);

        mRunningVoice = mv;
    }

    bool IsPlaying() const
    {
        // this function lets us delay for example, if there's a release stage
        // (theoretically)
        return mRunningVoice.IsPlaying();
    }

    void Unassign()
    {
        mRunningVoice.mVoiceId = MAGIC_VOICE_ID_UNASSIGNED;
    }

    Voice(int16_t vid)
        : mPatchOutDryLeft(mPannerSplitter, 0, CCSynthGraph::voiceMixerDryLeft, vid),
          mPatchOutDryRight(mPannerSplitter, 1, CCSynthGraph::voiceMixerDryRight, vid),
          mPatchOutDelayLeft(mPannerSplitter, 2, CCSynthGraph::delayInputMixerLeft, vid),
          mPatchOutDelayRight(mPannerSplitter, 3, CCSynthGraph::delayInputMixerRight, vid),
          mPatchOutVerbLeft(mPannerSplitter, 4, CCSynthGraph::verbInputMixerLeft, vid),
          mPatchOutVerbRight(mPannerSplitter, 5, CCSynthGraph::verbInputMixerLeft, vid)
    {
    }
};

Voice gVoices[MAX_SYNTH_VOICES] = {
    {0},
    {1},
    {2},
    {3},
    {4},
    {5},
};

struct SynthGraphControl
{
    float mPrevMetronomeBeatFrac = 0;
    AppSettings *mAppSettings;
    Metronome *mMetronome;

    void Setup(AppSettings *appSettings, Metronome *metronome /*, IModulationSourceSource *modulationSourceSource*/)
    {
        // AudioMemory(AUDIO_MEMORY_TO_ALLOCATE);
        AudioStream::initialize_memory(CLARINOID_AUDIO_MEMORY, SizeofStaticArray(CLARINOID_AUDIO_MEMORY));

        mAppSettings = appSettings;
        mMetronome = metronome;

        // for some reason patches really don't like to connect unless they are
        // last in the initialization order. Here's a workaround to force them to
        // connect.
        for (auto &v : gVoices)
        {
            v.EnsurePatchConnections(appSettings /*, modulationSourceSource*/);
        }

        CCSynthGraph::ampLeft.gain(1);
        CCSynthGraph::ampRight.gain(1);

        CCSynthGraph::metronomeEnv.delay(0);
        CCSynthGraph::metronomeEnv.attack(0);
        CCSynthGraph::metronomeEnv.hold(0);
        CCSynthGraph::metronomeEnv.releaseNoteOn(0);
        CCSynthGraph::metronomeEnv.sustain(0);
    }

    void BeginUpdate()
    {
        AudioNoInterrupts(); // https://www.pjrc.com/teensy/td_libs_AudioProcessorUsage.html
    }

    void EndUpdate()
    {
        AudioInterrupts();
    }

    void UpdatePostFx()
    {
        CCSynthGraph::delayFeedbackAmpLeft.gain(mAppSettings->mSynthSettings.mDelayFeedbackLevel);
        CCSynthGraph::delayFeedbackAmpRight.gain(mAppSettings->mSynthSettings.mDelayFeedbackLevel);
        CCSynthGraph::delayLeft.delay(0, mAppSettings->mSynthSettings.mDelayMS);
        CCSynthGraph::delayRight.delay(
            0, mAppSettings->mSynthSettings.mDelayMS + mAppSettings->mSynthSettings.mDelayStereoSep);

        CCSynthGraph::delayFilterLeft.SetParams(mAppSettings->mSynthSettings.mDelayFilterType,
                                                mAppSettings->mSynthSettings.mDelayCutoffFrequency,
                                                mAppSettings->mSynthSettings.mDelayQ,
                                                mAppSettings->mSynthSettings.mDelaySaturation);
        CCSynthGraph::delayFilterRight.SetParams(mAppSettings->mSynthSettings.mDelayFilterType,
                                                 mAppSettings->mSynthSettings.mDelayCutoffFrequency,
                                                 mAppSettings->mSynthSettings.mDelayQ,
                                                 mAppSettings->mSynthSettings.mDelaySaturation);

        CCSynthGraph::delayWetAmpLeft.gain(
            mAppSettings->mSynthSettings.mMasterFXEnable ? mAppSettings->mSynthSettings.mDelayGain : 0.0f);
        CCSynthGraph::delayWetAmpRight.gain(
            mAppSettings->mSynthSettings.mMasterFXEnable ? mAppSettings->mSynthSettings.mDelayGain : 0.0f);

        CCSynthGraph::verb.roomsize(mAppSettings->mSynthSettings.mReverbSize);
        CCSynthGraph::verb.damping(mAppSettings->mSynthSettings.mReverbDamping);

        CCSynthGraph::verbWetAmpLeft.gain(
            mAppSettings->mSynthSettings.mMasterFXEnable ? mAppSettings->mSynthSettings.mReverbGain : 0.0f);
        CCSynthGraph::verbWetAmpRight.gain(
            mAppSettings->mSynthSettings.mMasterFXEnable ? mAppSettings->mSynthSettings.mReverbGain : 0.0f);

        CCSynthGraph::ampLeft.gain(mAppSettings->mSynthSettings.mMasterGain);
        CCSynthGraph::ampRight.gain(mAppSettings->mSynthSettings.mMasterGain);

        if (!mAppSettings->mMetronomeSoundOn)
        {
            CCSynthGraph::metronomeOsc.amplitude(0);
        }
        else
        {
            CCSynthGraph::metronomeEnv.decay(mAppSettings->mMetronomeDecayMS);
            CCSynthGraph::metronomeOsc.amplitude(mAppSettings->mMetronomeGain);
            CCSynthGraph::metronomeOsc.frequency(MIDINoteToFreq(mAppSettings->mMetronomeNote));

            float metronomeBeatFrac = mMetronome->GetBeatFrac();
            if (metronomeBeatFrac < mPrevMetronomeBeatFrac)
            { // beat boundary is when the frac drops back
              // to 0
                CCSynthGraph::metronomeEnv.noteOn();
            }
            mPrevMetronomeBeatFrac = metronomeBeatFrac;
        }
    }
};

SynthGraphControl gSynthGraphControl;

} // namespace clarinoid
