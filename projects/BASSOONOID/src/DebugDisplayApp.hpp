#pragma once

#include <clarinoid/menu/MenuSettings.hpp>
#include <clarinoid/menu/Plotter.hpp>
#include "bsControlMapper.hpp"
#include "MusicalStateTask.hpp"

namespace clarinoid
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PerformanceApp : SettingsMenuApp
{
    size_t mSelectedTaskID = 0;
    TaskPlanner *mTaskManager = nullptr;
    MusicalStateTask *mpMusicalStateTask = nullptr;
    BassoonoidControlMapper *mpControls = nullptr;
    virtual const char *DisplayAppGetName() override
    {
        return "Performance";
    }

    SimpleMovingAverage<30> mCPUUsage;

    PerformanceApp(CCDisplay &d, MusicalStateTask *pMusicalStateTask, BassoonoidControlMapper *controls)
        : SettingsMenuApp(d), mpMusicalStateTask(pMusicalStateTask), mpControls(controls)
    {
    }

    MultiSubmenuSettingItem mTiming;

    LabelSettingItem mDelay = {
        Property<String>{[](void *cap) {
                             PerformanceApp *pThis = (PerformanceApp *)cap;
                             String ret = "TS Delay: ";
                             // ret += String((uint32_t)cap);
                             ret += (int)pThis->mTaskManager->mPreviousTimeSliceDelayTime.ElapsedMicros();
                             return ret;
                         },
                         this},
        AlwaysEnabled};

    LabelSettingItem mTimesliceLen = {
        Property<String>{[](void *cap) {
                             PerformanceApp *pThis = (PerformanceApp *)cap;
                             String ret = "TS Len: ";
                             ret += (int)pThis->mTaskManager->mTimesliceDuration.ElapsedMicros();
                             return ret;
                         },
                         this},
        AlwaysEnabled};

    LabelSettingItem mCPU = {
        Property<String>{[](void *cap) {
                             PerformanceApp *pThis = (PerformanceApp *)cap;
                             String ret = "CPU usage: ";
                             float p = (float)pThis->mTaskManager->mPreviousTimeSliceDelayTime.ElapsedMicros();
                             p /= pThis->mTaskManager->mTimesliceDuration.ElapsedMicros();
                             p = 1.0f - p;
                             pThis->mCPUUsage.Update(p);
                             p = pThis->mCPUUsage.GetValue() * 100;
                             ret += p;
                             ret += "%";
                             return ret;
                         },
                         this},
        AlwaysEnabled};

    LabelSettingItem mInput = {Property<String>{[](void *cap) {
                                                    PerformanceApp *pThis = (PerformanceApp *)cap;
                                                    String ret = "M->Input:";
                                                    ret += (int)pThis->mpMusicalStateTask->mInputTiming.GetValue();
                                                    return ret;
                                                },
                                                this},
                               AlwaysEnabled};

    LabelSettingItem mMusicalState = {
        Property<String>{[](void *cap) {
                             PerformanceApp *pThis = (PerformanceApp *)cap;
                             String ret = "M->Music:";
                             ret += (int)pThis->mpMusicalStateTask->mMusicalStateTiming.GetValue();
                             return ret;
                         },
                         this},
        AlwaysEnabled};

    LabelSettingItem mSynthState = {
        Property<String>{[](void *cap) {
                             PerformanceApp *pThis = (PerformanceApp *)cap;
                             String ret = "M->Synth:";
                             ret += (int)pThis->mpMusicalStateTask->mSynthStateTiming.GetValue();
                             return ret;
                         },
                         this},
        AlwaysEnabled};

    void Init(TaskPlanner *tm)
    {
        mTaskManager = tm;

        mTiming.Init(
            [](void *cap) { // get item count
                PerformanceApp *pThis = (PerformanceApp *)cap;
                return pThis->mTaskManager->mTasks.size();
            },
            [](void *cap, size_t i) // get name
            {
                PerformanceApp *pThis = (PerformanceApp *)cap;
                auto &t = pThis->mTaskManager->mTasks[i];
                String ret = String(t.mInfo.mName) + ":" + (int)t.mInfo.mExecutionTimeMicros.GetValue();
                // Serial.println(ret);
                return ret;
            },
            [](void *cap, size_t i) // get submenu
            {
                PerformanceApp *pThis = (PerformanceApp *)cap;
                pThis->mSelectedTaskID = i;
                return &pThis->mSubitemList;
            },
            [](void *cap, size_t i) { return true; },
            this);
    }

    LabelSettingItem mSubitemTmp = {Property<String>{[](void *cap) {
                                                         // PerformanceApp* pThis = (PerformanceApp*)cap;
                                                         return String("tmp.");
                                                     },
                                                     this},
                                    AlwaysEnabled};

    ISettingItem *mSubitemArray[1] = {
        &mSubitemTmp,
    };
    SettingsList mSubitemList = {mSubitemArray};

    ISettingItem *mArray[7] = {
        &mDelay,        // ok
        &mTimesliceLen, // ok
        &mCPU,          // definitely bugged
        &mTiming,       // ok
        &mInput,        // bugged
        &mMusicalState, // bugged
        &mSynthState,   // bugged
    };
    SettingsList mRootList = {mArray};
    virtual SettingsList *GetRootSettingsList()
    {
        return &mRootList;
    }

    virtual void RenderFrontPage()
    {
        mDisplay.mDisplay.println(String("PERFormance >"));
        mDisplay.mDisplay.println(String(" McpL: ") + (int)mpControls->mTimingMcpL.ElapsedMicros());
        mDisplay.mDisplay.println(String(" McpR: ") + (int)mpControls->mTimingMcpR.ElapsedMicros());
        mDisplay.mDisplay.println(String(" Breath: ") + (int)mpControls->mTimingBreath.ElapsedMicros());
        mDisplay.mDisplay.println(String(" Encoders: ") + (int)mpControls->mTimingEncoders.ElapsedMicros());
        mDisplay.mDisplay.println(String(" Analog: ") + (int)mpControls->mTimingAnalog.ElapsedMicros());
        mDisplay.mDisplay.println(String(" Digital: ") + (int)mpControls->mTimingDigital.ElapsedMicros());

        SettingsMenuApp::RenderFrontPage();
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct DebugDisplayApp : SettingsMenuApp
{
    virtual const char *DisplayAppGetName() override
    {
        return "DebugDisplayApp";
    }

    BassoonoidControlMapper &mControls;
    MusicalStateTask &mMusicalStateTask;

    DebugDisplayApp(CCDisplay &d, BassoonoidControlMapper &c, MusicalStateTask &mst)
        : SettingsMenuApp(d), mControls(c), mMusicalStateTask(mst)
    {
    }

    LabelSettingItem mMidiNote = {Property<String>{[](void *cap) {
                                                       DebugDisplayApp *pThis = (DebugDisplayApp *)cap;
                                                       String ret =
                                                           String("MidiNote: ") +
                                                           pThis->mMusicalStateTask.mMusicalState.mLiveVoice.mMidiNote;
                                                       return ret;
                                                   },
                                                   this},
                                  AlwaysEnabled};

    LabelSettingItem mFilterFreq = {
        Property<String>{
            [](void *cap) {
                DebugDisplayApp *pThis = (DebugDisplayApp *)cap;

                CCASSERT(pThis->mMusicalStateTask.mMusicalState.mLiveVoice.mSynthPatch >= 0);
                CCASSERT((size_t)pThis->mMusicalStateTask.mMusicalState.mLiveVoice.mSynthPatch < SYNTH_PRESET_COUNT);
                auto &synthPatch = pThis->mAppSettings->mSynthSettings
                                       .mPresets[pThis->mMusicalStateTask.mMusicalState.mLiveVoice.mSynthPatch];

                float filterFreq = clarinoid::Voice::CalcFilterCutoffFreq(
                    pThis->mMusicalStateTask.mMusicalState.mLiveVoice.mBreath01.GetFloatVal(),
                    pThis->mMusicalStateTask.mMusicalState.mLiveVoice.mMidiNote,
                    synthPatch.mFilterKeytracking,
                    synthPatch.mFilterMinFreq,
                    synthPatch.mFilterMaxFreq);

                String ret = String("Filter HZ: ") + filterFreq;
                return ret;
            },
            this},
        AlwaysEnabled};

    LabelSettingItem mBreath = {
        Property<String>{[](void *cap) {
                             DebugDisplayApp *pThis = (DebugDisplayApp *)cap;
                             return (String)(
                                 (String("Breath: ") + int(pThis->mControls.mBreath.CurrentValue01() * 1000)));
                         },
                         this},
        AlwaysEnabled};

    LabelSettingItem mLHA = {Property<String>{[](void *cap) {
                                                  DebugDisplayApp *pThis = (DebugDisplayApp *)cap;
                                                  String ret = "LHA:";
                                                  for (size_t i = 0; i < 8; ++i)
                                                  {
                                                      ret += pThis->mControls.mLHMCP.mButtons[i].CurrentValue()
                                                                 ? (String("") + i)
                                                                 : String(" ");
                                                  }
                                                  return ret;
                                              },
                                              this},
                             AlwaysEnabled};

    LabelSettingItem mLHB = {Property<String>{[](void *cap) {
                                                  DebugDisplayApp *pThis = (DebugDisplayApp *)cap;
                                                  String ret = "LHB:";
                                                  for (size_t i = 8; i < 16; ++i)
                                                  {
                                                      ret += pThis->mControls.mLHMCP.mButtons[i].CurrentValue()
                                                                 ? (String("") + i)
                                                                 : String(" ");
                                                  }
                                                  return ret;
                                              },
                                              this},
                             AlwaysEnabled};

    LabelSettingItem mRHA = {Property<String>{[](void *cap) {
                                                  DebugDisplayApp *pThis = (DebugDisplayApp *)cap;
                                                  String ret = "RHA:";
                                                  for (size_t i = 0; i < 8; ++i)
                                                  {
                                                      ret += pThis->mControls.mRHMCP.mButtons[i].CurrentValue()
                                                                 ? (String("") + i)
                                                                 : String(" ");
                                                  }
                                                  return ret;
                                              },
                                              this},
                             AlwaysEnabled};

    LabelSettingItem mRHB = {Property<String>{[](void *cap) {
                                                  DebugDisplayApp *pThis = (DebugDisplayApp *)cap;
                                                  String ret = "RHB:";
                                                  for (size_t i = 8; i < 16; ++i)
                                                  {
                                                      ret += pThis->mControls.mRHMCP.mButtons[i].CurrentValue()
                                                                 ? (String("") + i)
                                                                 : String(" ");
                                                  }
                                                  return ret;
                                              },
                                              this},
                             AlwaysEnabled};

    LabelSettingItem mCPEnc = {Property<String>{[](void *cap) {
                                                    DebugDisplayApp *pThis = (DebugDisplayApp *)cap;
                                                    String ret = String("CP Encoder raw:") +
                                                                 pThis->mControls.mCPEncoder.RawValue();
                                                    return ret;
                                                },
                                                this},
                               AlwaysEnabled};

    LabelSettingItem mLHEnc = {Property<String>{[](void *cap) {
                                                    DebugDisplayApp *pThis = (DebugDisplayApp *)cap;
                                                    String ret = String("LH Encoder raw:") +
                                                                 pThis->mControls.mLHEncoder.RawValue();
                                                    return ret;
                                                },
                                                this},
                               AlwaysEnabled};

    LabelSettingItem mRHEnc = {Property<String>{[](void *cap) {
                                                    DebugDisplayApp *pThis = (DebugDisplayApp *)cap;
                                                    String ret = String("RH Encoder raw:") +
                                                                 pThis->mControls.mRHEncoder.RawValue();
                                                    return ret;
                                                },
                                                this},
                               AlwaysEnabled};

    LabelSettingItem mToggleUp = {Property<String>{[](void *cap) {
                                                       DebugDisplayApp *pThis = (DebugDisplayApp *)cap;
                                                       String ret =
                                                           String("Toggle up:") +
                                                           (pThis->mControls.mToggleUp.CurrentValue() ? "1" : "0");
                                                       return ret;
                                                   },
                                                   this},
                                  AlwaysEnabled};

    LabelSettingItem mSynthPoly = {
        Property<String>{[](void *cap) {
                             DebugDisplayApp *pThis = (DebugDisplayApp *)cap;
                             String ret = String("Synth poly:") + (pThis->mMusicalStateTask.mSynth.mCurrentPolyphony);
                             return ret;
                         },
                         this},
        AlwaysEnabled};

    LabelSettingItem mAudioProcessorUsage = {Property<String>{[](void *cap) {
                                                                  // DebugDisplayApp* pThis = (DebugDisplayApp*)cap;
                                                                  String ret =
                                                                      String("Audio CPU %:") + AudioProcessorUsage();
                                                                  return ret;
                                                              },
                                                              this},
                                             AlwaysEnabled};

    LabelSettingItem mAudioProcessorUsageMax = {Property<String>{[](void *cap) {
                                                                     // DebugDisplayApp* pThis = (DebugDisplayApp*)cap;
                                                                     String ret = String("Audio max CPU %:") +
                                                                                  AudioProcessorUsageMax();
                                                                     return ret;
                                                                 },
                                                                 this},
                                                AlwaysEnabled};

    LabelSettingItem mAudioMemoryUsage = {Property<String>{[](void *cap) {
                                                               // DebugDisplayApp* pThis = (DebugDisplayApp*)cap;
                                                               String ret = String("Audio mem:") + AudioMemoryUsage();
                                                               return ret;
                                                           },
                                                           this},
                                          AlwaysEnabled};

    LabelSettingItem mAudioMemoryUsageMax = {Property<String>{[](void *cap) {
                                                                  // DebugDisplayApp* pThis = (DebugDisplayApp*)cap;
                                                                  String ret =
                                                                      String("Audio mem max:") + AudioMemoryUsageMax();
                                                                  return ret;
                                                              },
                                                              this},
                                             AlwaysEnabled};

    ISettingItem *mArray[16] = {
        &mMidiNote,
        &mFilterFreq,
        &mBreath,
        &mLHA,
        &mLHB,
        &mRHA,
        &mRHB,
        &mCPEnc,
        &mLHEnc,
        &mRHEnc,
        &mToggleUp,
        &mSynthPoly,
        &mAudioProcessorUsage,
        &mAudioProcessorUsageMax,
        &mAudioMemoryUsage,
        &mAudioMemoryUsageMax,
        // joyx
        // joyy
        // pitch
        // volume
    };
    SettingsList mRootList = {mArray};

    virtual SettingsList *GetRootSettingsList()
    {
        return &mRootList;
    }

    virtual void RenderFrontPage()
    {
        mDisplay.mDisplay.println(String("Debug info ->"));
        SettingsMenuApp::RenderFrontPage();
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct AudioMonitorApp : DisplayApp
{
    Plotter<MAX_DISPLAY_WIDTH> mPlotter;

    AudioMonitorApp(CCDisplay &d) : DisplayApp(d)
    {
        mPlotter.Plot(0);
    }

    virtual const char *DisplayAppGetName() override
    {
        return "AudioMonitor";
    }

    virtual void UpdateApp() override
    {
        if (mBack.IsNewlyPressed())
        {
            GoToFrontPage();
        }
        // float rms01 = CCSynthGraph::rms1.read();
        // CCSynthGraph::peak1.readPeakToPeak();
    }
    virtual void RenderApp() override
    {
        // mDisplay.mDisplay.setTextSize(1);
        // mDisplay.mDisplay.setCursor(0, 0);
        // mDisplay.mDisplay.setTextWrap(false);
        // mDisplay.mDisplay.println(String("woa"));
    }
    virtual void RenderFrontPage() override
    {
        mDisplay.mDisplay.println(String("Peak"));
        float peak = CCSynth::GetPeakLevel();
        mPlotter.Plot(peak);
        RectI rcDisplay = {0, 0, this->mDisplay.mDisplay.width(), this->mDisplay.mDisplay.height()};
        mPlotter.Render(this->mDisplay, rcDisplay);
    }

    virtual void DisplayAppUpdate() override
    {
        DisplayApp::DisplayAppUpdate(); // update input
    }
};

} // namespace clarinoid
