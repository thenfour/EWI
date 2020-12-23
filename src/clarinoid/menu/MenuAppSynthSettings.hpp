#pragma once

#include "MenuAppBase.hpp"
#include "MenuSettings.hpp"
#include "NumericSettingItem.hpp"
#include "EnumSettingItem.hpp"

namespace clarinoid
{

    struct SynthPatchMenuApp
    {
        size_t mEditingSynthPatch = 0;
        CCDisplay &mDisplay;

        SynthPatchMenuApp(CCDisplay &d) : mDisplay(d) {}

        AppSettings &GetAppSettings() { return *mDisplay.mAppSettings; }
        SynthPreset &GetBinding() { return GetAppSettings().mSynthSettings.mPresets[mEditingSynthPatch]; }

        // NumericSettingItem(const String& name, T min_, T max_, const Property<T>& binding, typename cc::function<bool()>::ptr_t isEnabled) :
        // BoolSettingItem(const String& name, const String& trueCaption, const String& falseCaption, const Property<bool>& binding, typename cc::function<bool()>::ptr_t isEnabled) :
        FloatSettingItem mDetune = {"Detune", StandardRangeSpecs::gFloat_0_2,
                                    Property<float>{
                                        [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mDetune; },
                                        [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mDetune = v; },
                                        this},
                                    AlwaysEnabled};

        FloatSettingItem mPortamentoTime = {"Portamento", StandardRangeSpecs::gPortamentoRange,
                                            Property<float>{
                                                [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mPortamentoTime; },
                                                [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mPortamentoTime = v; },
                                                this},
                                            AlwaysEnabled};

        BoolSettingItem mSync = {"Sync", "On", "Off",
                                 Property<bool>{
                                     [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mSync; },
                                     [](void *cap, const bool &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mSync = v; },
                                     this},
                                 AlwaysEnabled};


        FloatSettingItem mSyncMultMin = {"Sync*>", NumericEditRangeSpec<float> { 0.0f, 15.0f },
                                            Property<float>{
                                                [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mSyncMultMin; },
                                                [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mSyncMultMin = v; },
                                                this},
                                            AlwaysEnabled};

        FloatSettingItem mSyncMultMax = {"Sync*<", NumericEditRangeSpec<float> { 0.0f, 15.0f },
                                            Property<float>{
                                                [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mSyncMultMax; },
                                                [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mSyncMultMax = v; },
                                                this},
                                            AlwaysEnabled};

        FloatSettingItem mBreathFiltMin = {"Breath filt >", NumericEditRangeSpec<float> { 0.0f, 3000.0f },
                                            Property<float>{
                                                [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mFilterMinFreq; },
                                                [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mFilterMinFreq = v; },
                                                this},
                                            AlwaysEnabled};

        FloatSettingItem mBreathFiltMax = {"Breath filt <", NumericEditRangeSpec<float> { 200.0f, 15000.0f },
                                            Property<float>{
                                                [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mFilterMaxFreq; },
                                                [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mFilterMaxFreq = v; },
                                                this},
                                            AlwaysEnabled};

        FloatSettingItem mBreathFiltQ = {"Breath Q ", NumericEditRangeSpec<float> { 0.7f, 5.0f },
                                            Property<float>{
                                                [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mFilterQ; },
                                                [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mFilterQ = v; },
                                                this},
                                            AlwaysEnabled};


        // EnumSettingItem(const String& name, const EnumInfo<T>& enumInfo, const Property<T>& binding, cc::function<bool()>::ptr_t isEnabled) :
        EnumSettingItem<OscWaveformShape> mOsc1Waveform = {"mOsc1WaveformType", gOscWaveformShapeInfo,
                                                           Property<OscWaveformShape>{
                                                               [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mOsc1Waveform; },
                                                               [](void *cap, const OscWaveformShape &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mOsc1Waveform = v; },
                                                               this},
                                                           AlwaysEnabled};

        FloatSettingItem mOsc1Gain = {"Osc1Gain", StandardRangeSpecs::gFloat_0_1,
                                      Property<float>{
                                          [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mOsc1Gain; },
                                          [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mOsc1Gain = v; },
                                          this},
                                      AlwaysEnabled};

        IntSettingItem mOsc1PitchSemis = {"mOsc1PitchSemis", StandardRangeSpecs::gTransposeRange,
                                          Property<int>{
                                              [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mOsc1PitchSemis; },
                                              [](void *cap, const int &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mOsc1PitchSemis = v; },
                                              this},
                                          AlwaysEnabled};
        FloatSettingItem mOsc1PitchFine = {"mOsc1PitchFine", StandardRangeSpecs::gFloat_0_1,
                                           Property<float>{
                                               [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mOsc1PitchFine; },
                                               [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mOsc1PitchFine = v; },
                                               this},
                                           AlwaysEnabled};
        FloatSettingItem mOsc1PulseWidth = {"mOsc1PulseWidth", StandardRangeSpecs::gFloat_0_1,
                                            Property<float>{
                                                [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mOsc1PulseWidth; },
                                                [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mOsc1PulseWidth = v; },
                                                this},
                                            AlwaysEnabled};

        EnumSettingItem<OscWaveformShape> mOsc2Waveform = {"mOsc2WaveformType", gOscWaveformShapeInfo,
                                                           Property<OscWaveformShape>{
                                                               [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mOsc2Waveform; },
                                                               [](void *cap, const OscWaveformShape &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mOsc2Waveform = v; },
                                                               this},
                                                           AlwaysEnabled};
        FloatSettingItem mOsc2Gain = {"Osc2Gain", StandardRangeSpecs::gFloat_0_1,
                                      Property<float>{
                                          [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mOsc2Gain; },
                                          [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mOsc2Gain = v; },
                                          this},
                                      AlwaysEnabled};
        IntSettingItem mOsc2PitchSemis = {"mOsc2PitchSemis", StandardRangeSpecs::gTransposeRange,
                                          Property<int>{
                                              [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mOsc2PitchSemis; },
                                              [](void *cap, const int &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mOsc2PitchSemis = v; },
                                              this},
                                          AlwaysEnabled};
        FloatSettingItem mOsc2PitchFine = {"mOsc2PitchFine", StandardRangeSpecs::gFloat_0_1,
                                           Property<float>{
                                               [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mOsc2PitchFine; },
                                               [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mOsc2PitchFine = v; },
                                               this},
                                           AlwaysEnabled};
        FloatSettingItem mOsc2PulseWidth = {"mOsc2PulseWidth", StandardRangeSpecs::gFloat_0_1,
                                            Property<float>{
                                                [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mOsc2PulseWidth; },
                                                [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mOsc2PulseWidth = v; },
                                                this},
                                            AlwaysEnabled};

        EnumSettingItem<OscWaveformShape> mOsc3Waveform = {"mOsc3Waveform", gOscWaveformShapeInfo,
                                                           Property<OscWaveformShape>{
                                                               [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mOsc3Waveform; },
                                                               [](void *cap, const OscWaveformShape &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mOsc3Waveform = v; },
                                                               this},
                                                           AlwaysEnabled};
        FloatSettingItem mOsc3Gain = {"mOsc3Gain", StandardRangeSpecs::gFloat_0_1,
                                      Property<float>{
                                          [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mOsc3Gain; },
                                          [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mOsc3Gain = v; },
                                          this},
                                      AlwaysEnabled};
        IntSettingItem mOsc3PitchSemis = {"mOsc3PitchSemis", StandardRangeSpecs::gTransposeRange,
                                          Property<int>{
                                              [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mOsc3PitchSemis; },
                                              [](void *cap, const int &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mOsc3PitchSemis = v; },
                                              this},
                                          AlwaysEnabled};
        FloatSettingItem mOsc3PitchFine = {"mOsc3PitchFine", StandardRangeSpecs::gFloat_0_1,
                                           Property<float>{
                                               [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mOsc3PitchFine; },
                                               [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mOsc3PitchFine = v; },
                                               this},
                                           AlwaysEnabled};
        FloatSettingItem mOsc3PulseWidth = {"mOsc3PulseWidth", StandardRangeSpecs::gFloat_0_1,
                                            Property<float>{
                                                [](void *cap) { auto* pThis = (SynthPatchMenuApp*)cap; return pThis->GetBinding().mOsc3PulseWidth; },
                                                [](void *cap, const float &v) { auto* pThis = (SynthPatchMenuApp*)cap; pThis->GetBinding().mOsc3PulseWidth = v; },
                                                this},
                                            AlwaysEnabled};

        ISettingItem *mArray[23] =
            {
                &mDetune,
                &mPortamentoTime,
                &mSync,
                &mSyncMultMin,
                &mSyncMultMax,
                &mBreathFiltMin,
                &mBreathFiltMax,
                &mBreathFiltQ,
                &mOsc1Waveform,
                &mOsc1Gain,
                &mOsc1PitchSemis,
                &mOsc1PitchFine,
                &mOsc1PulseWidth,
                &mOsc2Waveform,
                &mOsc2Gain,
                &mOsc2PitchSemis,
                &mOsc2PitchFine,
                &mOsc2PulseWidth,
                &mOsc3Waveform,
                &mOsc3Gain,
                &mOsc3PitchSemis,
                &mOsc3PitchFine,
                &mOsc3PulseWidth,
        };
        SettingsList mRootList = {mArray};

        SettingsList *Start(size_t iPatch)
        {
            mEditingSynthPatch = iPatch;
            return &mRootList;
        }
    };

    struct SynthSettingsApp : public SettingsMenuApp
    {
        virtual const char *DisplayAppGetName() override { return "SynthSettingsApp"; }

        SynthPatchMenuApp mSynthPatchSettingsApp;
        SynthSettings &GetSynthSettings() { return this->mAppSettings->mSynthSettings; }

        SynthSettingsApp(CCDisplay &d) : SettingsMenuApp(d), mSynthPatchSettingsApp(d)
        {
        }

        FloatSettingItem mMasterGain = {"Master gain", StandardRangeSpecs::gFloat_0_2,
                                        Property<float>{
                                            [](void *cap) { auto *pThis = (SynthSettingsApp *)cap; return pThis->GetSynthSettings().mMasterGain; },
                                            [](void *cap, const float &v) { auto *pThis = (SynthSettingsApp *)cap; pThis->GetSynthSettings().mMasterGain = v; },
                                            this},
                                        AlwaysEnabled};

        IntSettingItem mGlobalSynthPreset = {"Global synth P#", NumericEditRangeSpec<int> { 0, SYNTH_PRESET_COUNT - 1 },
                                             Property<int>{
                                                 [](void *cap) { auto *pThis = (SynthSettingsApp *)cap; return (int)pThis->GetAppSettings()->mGlobalSynthPreset; },
                                                 [](void *cap, const int &v) { auto *pThis = (SynthSettingsApp *)cap; pThis->GetAppSettings()->mGlobalSynthPreset = (uint16_t)v; },
                                                 this},
                                             AlwaysEnabled};

        IntSettingItem mTranspose = {"Transpose", StandardRangeSpecs::gTransposeRange,
                                     Property<int>{
                                         [](void *cap) { auto *pThis = (SynthSettingsApp *)cap; return pThis->GetAppSettings()->mTranspose; },
                                         [](void *cap, const int &v) { auto *pThis = (SynthSettingsApp *)cap; pThis->GetAppSettings()->mTranspose = v; },
                                         this},
                                     AlwaysEnabled};

        FloatSettingItem mReverbGain = {"Reverb gain", StandardRangeSpecs::gFloat_0_1,
                                        Property<float>{
                                            [](void *cap) { auto *pThis = (SynthSettingsApp *)cap; return pThis->GetSynthSettings().mReverbGain; },
                                            [](void *cap, const float &v) { auto *pThis = (SynthSettingsApp *)cap; pThis->GetSynthSettings().mReverbGain = v; },
                                            this},
                                        AlwaysEnabled};

        MultiSubmenuSettingItem mPatches = {
            [](void *cap) { return SYNTH_PRESET_COUNT; },
            [](void *cap, size_t n) {
                auto *pThis = (SynthSettingsApp *)cap;
                if (pThis->GetSynthSettings().mPresets[n].mName.length() == 0)
                {
                    return String(String("Preset ") + n);
                }
                return pThis->GetSynthSettings().mPresets[n].mName;
            },                           // name
            [](void *cap, size_t n) { auto* pThis = (SynthSettingsApp*)cap; return pThis->mSynthPatchSettingsApp.Start(n); }, // get submenu list
            [](void *cap, size_t n) { return true; },
            (void *)this // capture
        };

        ISettingItem *mArray[5] =
            {
                &mMasterGain,
                &mGlobalSynthPreset,
                &mTranspose,
                &mReverbGain,
                &mPatches,
            };
        SettingsList mRootList = {mArray};

    public:
        virtual SettingsList *GetRootSettingsList()
        {
            return &mRootList;
        }

        virtual void RenderFrontPage()
        {
            mDisplay.mDisplay.setTextSize(1);
            mDisplay.mDisplay.setTextColor(WHITE);
            mDisplay.mDisplay.setCursor(0, 0);

            mDisplay.mDisplay.println(String("SYNTH SETTINGS"));
            mDisplay.mDisplay.println(String(""));
            mDisplay.mDisplay.println(String(""));
            mDisplay.mDisplay.println(String("                  -->"));

            SettingsMenuApp::RenderFrontPage();
        }
    };

} // namespace clarinoid
