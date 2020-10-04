
#pragma once

#include <clarinoid/basic/Basic.hpp>

static const size_t MAX_SYNTH_VOICES = 8;

static const size_t HARM_PRESET_COUNT = 16;
static const size_t HARM_VOICES = 6;
static const size_t HARM_SEQUENCE_LEN = 8;

static const size_t LOOP_LAYERS = 6;
static constexpr size_t MAX_MUSICAL_VOICES = LOOP_LAYERS * (HARM_VOICES + 1 /* each harmonized preset can also output the playing (live) note as well, so make room.*/);

static const size_t PRESET_NAME_LEN = 16;

static const size_t SYNTH_PRESET_COUNT = 16;

bool gTouchKeyGraphsIsRunning = false; // todo: this is a hack


#include "HarmonizerSettings.hpp"
#include "SynthSettings.hpp"
#include "LoopstationSettings.hpp"




enum class GlobalScaleRefType : uint8_t
{
  Chosen,
  Deduced,
};

EnumItemInfo<GlobalScaleRefType> gGlobalScaleRefTypeItems[2] = {
  { GlobalScaleRefType::Chosen, "Chosen" },
  { GlobalScaleRefType::Deduced, "Deduced" },
};

EnumInfo<GlobalScaleRefType> gGlobalScaleRefTypeInfo ("GlobalScaleRefType", gGlobalScaleRefTypeItems);



// the pitch bend strip has various zones.
//                 | IDLE  | UP MAX | UP VARIABLE     | ZERO       | DOWN VARIABLE                | DOWN MAX  |
//                 |-------|--------|-----------------|------------|------------------------------|-----------|
// analog value:  Latch   0.03      0.08              0.20         0.38                           0.88        1.00
// IDLE =               pitch is unchanged (latching behavior issue #30)
// UPMAX =              pitch is +1
// UP VARIABLE = transition from 0 -> 1
// ZERO =               pitch is 0
// DOWN VARIABLE = transition from 0 -> -1
// DOWN MAX     =       pitch is -1
// 
// * the UP MAX range is very important, to have some buffer between the hard cutoff of IDLE to UPMAX.
//   maybe even a small transition zone would be useful to a player as a warning.
struct PitchStripSettings
{
  float mHandsOffNoiseThresh = 0.035f;
  float mPitchUpMax = 0.08f; // or min? depends how you measure this.
  float mZeroMin = 0.20f;
  float mZeroMax = 0.38f;
  float mPitchDownMax = 0.88f;
};


struct AppSettings
{
  bool mDisplayDim = true;
  bool mOrangeLEDs = false;

  float mBreathLowerBound = 0.07f;
  float mBreathUpperBound = 0.7f;
  float mBreathNoteOnThreshold = 0.01f;
  
  float mTouchMaxFactor = 1.5f;

  PitchStripSettings mPitchStrip;

  float mReverbGain = 0.2f;

  bool mMetronomeOn = false;
  float mMetronomeGain = 0.8f;
  int mMetronomeNote = 80;
  int mMetronomeDecayMS= 15;

  float mBPM = 104.0f;

  int mTranspose = 0;

  GlobalScaleRefType mGlobalScaleRef = GlobalScaleRefType::Deduced;
  Scale mGlobalScale = Scale { Note::E, ScaleFlavorIndex::MajorPentatonic }; // you can set this in menus
  Scale mDeducedScale = Scale { Note::C, ScaleFlavorIndex::MajorPentatonic };; // this is automatically populated always
  
  HarmSettings mHarmSettings;
  LooperSettings mLooperSettings;
  SynthSettings mSynthSettings;

  // these are for the live playing voice. a harmonizer's voices can override the synth preset though.
  uint16_t mGlobalSynthPreset = 0;
  uint16_t mGlobalHarmPreset = 2;
};

AppSettings gAppSettings;


static constexpr auto appsettingssize = sizeof(gAppSettings);


///////////////////////////////////////////////////////////////////////////////////////////////////
HarmPreset& FindHarmPreset(uint16_t id) {
  if (id < 0) return gAppSettings.mHarmSettings.mDisabledPreset;
  if (id >= SYNTH_PRESET_COUNT) return gAppSettings.mHarmSettings.mDisabledPreset;
  return gAppSettings.mHarmSettings.mPresets[id];
}

SynthPreset& FindSynthPreset(uint16_t id)
{
  if (id < 0) id = 0;
  if (id >= SYNTH_PRESET_COUNT) id = 0;
  return gAppSettings.mSynthSettings.mPresets[id];
}

