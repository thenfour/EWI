#pragma once

#include <clarinoid/basic/Basic.hpp>
#include "MenuAppBase.hpp"
#include "MenuListControl.hpp"

namespace clarinoid
{


template<typename T>
struct EnumEditor : ISettingItemEditor
{
  // set on SetupEditing()
  ISettingItemEditorActions* mpApi;
  T mOldVal;

  Property<T> mBinding;
  const EnumInfo<T>& mEnumInfo;

  Property<int> mListSelectedItem = {
    [](void* capture) { EnumEditor<T>* pthis = (EnumEditor<T>*)capture; return (int)pthis->mBinding.GetValue(); },
    [](void* capture, const int& val) { EnumEditor<T>* pthis = (EnumEditor<T>*)capture; pthis->mBinding.SetValue((T)val); },
    this
  };

  ListControl mListControl;

  EnumEditor(const EnumInfo<T>&enumInfo, const Property<T>& binding) :
    mBinding(binding),
    mEnumInfo(enumInfo),
    mListControl(&mEnumInfo, mListSelectedItem, 12, 12, 3)
  {
  }

  virtual void SetupEditing(ISettingItemEditorActions* papi, int x, int y) {
    mpApi = papi;
    if (!mpApi) mpApi = this;
    mOldVal = mBinding.GetValue();
  }

  // for editing when holding back button. so back button is definitely pressed, and we don't want to bother
  // with encoder button pressed here.
  virtual void UpdateMomentaryMode(int encIntDelta)
  {
  }
  
  virtual void Update(bool backWasPressed, bool encWasPressed, int encIntDelta)
  {
    CCASSERT(mpApi != nullptr);
    //Serial.println(String("update enum editor ") + (int)(micros()));
    mListControl.Update();
    if (backWasPressed) {
      mBinding.SetValue(mOldVal);
      mpApi->CommitEditing();
    }
    if (encWasPressed) {
      mpApi->CommitEditing();
    }
  }
  virtual void Render()
  {
    mpApi->GetDisplay()->SetupModal();
    mListControl.Render();
  }
};


template<typename T>
struct EnumSettingItem : public ISettingItem
{
  String mName;
  EnumEditor<T> mEditor;
  Property<T> mBinding;
  const EnumInfo<T>& mEnumInfo;
  cc::function<bool()>::ptr_t mIsEnabled;

  EnumSettingItem(const String& name, const EnumInfo<T>& enumInfo, const Property<T>& binding, cc::function<bool()>::ptr_t isEnabled) :
    mName(name),
    mEditor(enumInfo, binding),
    mBinding(binding),
    mEnumInfo(enumInfo),
    mIsEnabled(isEnabled)
  {
  }

  virtual String GetName(size_t multiIndex) { return mName; }
  virtual String GetValueString(size_t multiIndex) { return mEnumInfo.GetValueString(mBinding.GetValue()); }
  virtual SettingItemType GetType(size_t multiIndex) { return SettingItemType::Custom; }
  virtual bool IsEnabled(size_t multiIndex) const { return mIsEnabled(); }
  virtual ISettingItemEditor* GetEditor(size_t multiIndex) {
    return &mEditor;
  }
};


} // namespace clarinoid