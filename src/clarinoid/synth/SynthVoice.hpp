
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

namespace clarinoid
{

namespace CCSynthGraph
{
/*
https://www.pjrc.com/teensy/gui/index.html
// this is the after-oscillator processing.

*/

// GUItool: begin automatically generated code
AudioAmplifier delayFeedbackAmpLeft;  // xy=376.00567626953125,85
AudioEffectDelay delayLeft;           // xy=378.00567626953125,176
AudioEffectDelay delayRight;          // xy=388.00567626953125,406
AudioAmplifier delayFeedbackAmpRight; // xy=389.00567626953125,318
AudioMixer4 verbInputMixer;           // xy=761.0056762695312,476
AudioSynthWaveformSine metronomeOsc;  // xy=797.0056762695312,905
AudioEffectFreeverbStereo verb;       // xy=911.0056762695312,482
AudioEffectEnvelope metronomeEnv;     // xy=998.0056762695312,907
AudioAmplifier verbWetAmpLeft;        // xy=1070.0056762695312,463
AudioAmplifier verbWetAmpRight;       // xy=1077.0056762695312,501
AudioMixer4 postMixerLeft;            // xy=1252.0056762695312,690
AudioMixer4 postMixerRight;           // xy=1254.0056762695312,773
AudioAmplifier ampLeft;               // xy=1414.0056762695312,691
AudioAmplifier ampRight;              // xy=1417.0056762695312,772
AudioOutputI2S i2s1;                  // xy=1599.0056762695312,724
AudioAnalyzePeak peak1;               // xy=1642.0056762695312,503
AudioConnection patchCord1(verbInputMixer, verb);
AudioConnection patchCord2(metronomeOsc, metronomeEnv);
AudioConnection patchCord3(verb, 0, verbWetAmpLeft, 0);
AudioConnection patchCord4(verb, 1, verbWetAmpRight, 0);
AudioConnection patchCord5(metronomeEnv, 0, postMixerRight, 3);
AudioConnection patchCord6(metronomeEnv, 0, postMixerLeft, 3);
AudioConnection patchCord7(verbWetAmpLeft, 0, postMixerLeft, 2);
AudioConnection patchCord8(verbWetAmpRight, 0, postMixerRight, 2);
AudioConnection patchCord9(postMixerLeft, ampLeft);
AudioConnection patchCord10(postMixerRight, ampRight);
AudioConnection patchCord11(ampLeft, peak1);
AudioConnection patchCord12(ampLeft, 0, i2s1, 1);
AudioConnection patchCord13(ampRight, 0, i2s1, 0);
// GUItool: end automatically generated code

// insert the delay filters.
// ...[delayLeft]---->[delayFilterLeft]--------------->[delayFeedbackAmpLeft]...
//                                     \-------------->[waveMixerLeft]...
::clarinoid::FilterNode delayFilterLeft;
AudioConnection mPatchDelayToFilterLeft = {delayLeft, 0, delayFilterLeft, 0};
AudioConnection mPatchDelayFilterToFeedbackAmpLeft = {delayFilterLeft, 0, delayFeedbackAmpLeft, 0};

::clarinoid::FilterNode delayFilterRight;
AudioConnection mPatchDelayToFilterRight = {delayRight, 0, delayFilterRight, 0};
AudioConnection mPatchDelayFilterToFeedbackAmpRight = {delayFilterRight, 0, delayFeedbackAmpRight, 0};

// delay output connection
AudioConnection mPatchDelayFilterToWaveMixerLeft = {delayFilterLeft, 0, postMixerLeft, 1};
AudioConnection mPatchDelayFilterToWaveMixerRight = {delayFilterRight, 0, postMixerRight, 1};

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

    CCPatch mPatchBreathToMod = {mBreathModSource, 0, mModMatrix, (uint8_t)ModulationSourceToIndex(ModulationSource::Breath)};
    CCPatch mPatchPitchStripToMod = {mPitchBendModSource, 0, mModMatrix, (uint8_t)ModulationSourceToIndex(ModulationSource::PitchStrip)};

    // Patch modulation destinations
    /*
Input 0: Frequency Modulation for Oscillator 1
Input 1: Pulse Width Modulation for Oscillator 1
Input 2: Frequency Modulation for Oscillator 2
Input 3: Pulse Width Modulation for Oscillator 2
Input 4: Frequency Modulation for Oscillator 3
Input 5: Pulse Width Modulation for Oscillator 3
    */
    CCPatch mPatchModToOsc1PWM = {mModMatrix, (uint8_t)ModulationDestinationToIndex(ModulationDestination::Osc1PulseWidth), mOsc, 1};
    CCPatch mPatchModToOsc2PWM = {mModMatrix, (uint8_t)ModulationDestinationToIndex(ModulationDestination::Osc1PulseWidth), mOsc, 3};
    CCPatch mPatchModToOsc3PWM = {mModMatrix, (uint8_t)ModulationDestinationToIndex(ModulationDestination::Osc1PulseWidth), mOsc, 5};

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

    void EnsurePatchConnections(AppSettings *appSettings)
    {
        mAppSettings = appSettings;

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
        mEnv1.delay(mPreset->mEnv1Delay);
        mEnv1.attack(mPreset->mEnv1Attack);
        mEnv1.decay(mPreset->mEnv1Decay);
        mEnv1.sustain(mPreset->mEnv1Sustain);
        mEnv1.release(mPreset->mEnv1Release);

        mEnv2.delay(mPreset->mEnv1Delay);
        mEnv2.attack(mPreset->mEnv1Attack);
        mEnv2.decay(mPreset->mEnv1Decay);
        mEnv2.sustain(mPreset->mEnv1Sustain);
        mEnv2.release(mPreset->mEnv1Release);


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

        mOsc.amplitude(1,mPreset->mOsc1Gain);
        mOsc.amplitude(2,mPreset->mOsc2Gain);
        mOsc.amplitude(3,mPreset->mOsc3Gain);

        // update
        float midiNote =
            (float)mv.mMidiNote + mv.mPitchBendN11.GetFloatVal() * mAppSettings->mSynthSettings.mPitchBendRange;

        mOsc.portamentoTime(1, mPreset->mPortamentoTime);
        mOsc.portamentoTime(2, mPreset->mPortamentoTime);
        mOsc.portamentoTime(3, mPreset->mPortamentoTime);

        mOsc.waveform(1, (uint8_t)mPreset->mOsc1Waveform);
        mOsc.waveform(2, (uint8_t)mPreset->mOsc2Waveform);
        mOsc.waveform(3, (uint8_t)mPreset->mOsc3Waveform);

        mOsc.pulseWidth(1, mPreset->mOsc1PulseWidth);
        mOsc.pulseWidth(2, mPreset->mOsc2PulseWidth);
        mOsc.pulseWidth(3, mPreset->mOsc3PulseWidth);

        // pwm amount is always 1 within the polyblep osc; actual PWM amount is controlled by modulation matrix.
        mOsc.pwmAmount(1, 1);
        mOsc.pwmAmount(2, 1);
        mOsc.pwmAmount(3, 1);

        mOsc.frequency(
            1, MIDINoteToFreq(midiNote + mPreset->mOsc1PitchFine + mPreset->mOsc1PitchSemis - mPreset->mDetune));
        mOsc.frequency(
            3, MIDINoteToFreq(midiNote + mPreset->mOsc3PitchFine + mPreset->mOsc3PitchSemis + mPreset->mDetune));

        if (mPreset->mSync)
        {
            float freq = MIDINoteToFreq(midiNote + mPreset->mOsc2PitchFine + mPreset->mOsc2PitchSemis);
            float freqSync =
                map(mv.mBreath01.GetFloatVal(), 0.0f, 1.0f, freq * mPreset->mSyncMultMin, freq * mPreset->mSyncMultMax);
            mOsc.frequency(2, freqSync);
        }
        else
        {
            mOsc.frequency(2, MIDINoteToFreq(midiNote + mPreset->mOsc2PitchFine + mPreset->mOsc2PitchSemis));
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

    void Setup(AppSettings *appSettings, Metronome *metronome/*, IModulationSourceSource *modulationSourceSource*/)
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
            v.EnsurePatchConnections(appSettings/*, modulationSourceSource*/);
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

        CCSynthGraph::verb.roomsize(mAppSettings->mSynthSettings.mReverbSize);
        CCSynthGraph::verb.damping(mAppSettings->mSynthSettings.mReverbDamping);

        CCSynthGraph::verbWetAmpLeft.gain(mAppSettings->mSynthSettings.mReverbGain);
        CCSynthGraph::verbWetAmpRight.gain(mAppSettings->mSynthSettings.mReverbGain);

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
