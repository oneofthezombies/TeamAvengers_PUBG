#pragma once

class UITest : public UIObject
{
    UIImage* pImage;
    UIImage* pAlpha;

public:
             UITest();
    virtual ~UITest();

    virtual void Render() override;
};