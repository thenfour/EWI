#pragma once

#include <clarinoid/basic/Basic.hpp>
#include "MenuAppBase.hpp"
#include "MenuListControl.hpp"

namespace clarinoid
{

struct BoolSettingItem : public ISettingItem
{
    String mName;
    Property<bool> mBinding;
    String mTrueCaption;
    String mFalseCaption;
    Property<bool> mIsEnabled;

    BoolSettingItem(const String &name,
                    const String &trueCaption,
                    const String &falseCaption,
                    const Property<bool> &binding,
                    const Property<bool> &isEnabled)
        : mName(name), mBinding(binding), mTrueCaption(trueCaption), mFalseCaption(falseCaption), mIsEnabled(isEnabled)
    {
    }

    virtual String GetName(size_t multiIndex)
    {
        return mName;
    }
    virtual String GetValueString(size_t multiIndex)
    {
        return mBinding.GetValue() ? mTrueCaption : mFalseCaption;
    }
    virtual bool IsEnabled(size_t multiIndex) const
    {
        return mIsEnabled.GetValue();
    }
    virtual SettingItemType GetType(size_t multiIndex)
    {
        return SettingItemType::Bool;
    }
    virtual void ToggleBool(size_t multiIndex)
    {
        mBinding.SetValue(!mBinding.GetValue());
    }
};

} // namespace clarinoid
