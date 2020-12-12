

#pragma once

namespace clarinoid
{


//////////////////////////////////////////////////////////////////////
struct ListControl
{
  const IList* mpList;
  Property<int> mSelectedItem;
  int mX;
  int mY;
  int mMaxItemsToRender;
  EncoderReader mEnc;// = EncoderReader { gControlMapper.MenuEncoder() };
  CCDisplay* mDisplay;
  IEncoder* pEncoder;

  ListControl(const IList* list, CCDisplay* d, IEncoder* penc, Property<int> selectedItemBinding, int x, int y, int nVisibleItems) : 
    mpList(list),
    mSelectedItem(selectedItemBinding),
    mX(x),
    mY(y),
    mMaxItemsToRender(nVisibleItems),
    mDisplay(d),
    pEncoder(penc)
  {
  }

  void Init()
  {
    //mEnc.SetSource(pEncoder);
    //mEnc.SetSource(mControlMapper->MenuEncoder());
  }
  
  void Render()
  {
    auto count = mpList->List_GetItemCount();
    if (count == 0) return;
    mDisplay->mDisplay.setTextSize(1);
    //gDisplay.mDisplay.setCursor(0, 0);
    mDisplay->mDisplay.setTextWrap(false);
    int itemToRender = RotateIntoRange(mSelectedItem.GetValue() - 1, count);
    const int itemsToRender = min(mMaxItemsToRender, count);
    for (int i = 0; i < itemsToRender; ++ i) {
      if (itemToRender == mSelectedItem.GetValue()) {
        mDisplay->mDisplay.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
      } else {
        mDisplay->mDisplay.setTextColor(SSD1306_WHITE, SSD1306_BLACK); // normal text
      }

      mDisplay->mDisplay.println(mpList->List_GetItemCaption(itemToRender));

      itemToRender = RotateIntoRange(itemToRender + 1, count);
    }
  }
  
  virtual void Update()
  {
    mEnc.Update(pEncoder);
    CCASSERT(mpList);
    auto c = mpList->List_GetItemCount();
    if (c == 0)
      return;
    //auto v = mSelectedItem.GetValue();
    mSelectedItem.SetValue(AddConstrained(mSelectedItem.GetValue(), mEnc.GetIntDelta(), 0, mpList->List_GetItemCount() - 1));
  }
};


} // namespace clarinoid
