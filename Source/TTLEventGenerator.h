#ifndef __TTLEVENTGENERATOR_H__
#define __TTLEVENTGENERATOR_H__

#include <ProcessorHeaders.h>

class TTLEventGenerator : public GenericProcessor
{
public:
    TTLEventGenerator();
    ~TTLEventGenerator();

    void process(AudioBuffer<float>& buffer) override;
    void parameterValueChanged(Parameter* param) override;
    void updateSettings() override;
    bool startAcquisition() override;
    
    AudioProcessorEditor* createEditor() override;

private:
    EventChannel* ttlChannel;
    int counter = 0; 
    bool state = false; 

    // 가이드 하단 UI 연동을 위한 변수들
    bool shouldTriggerEvent = false;
    bool eventWasTriggered = false;
    int triggeredEventCounter = 0;
    float eventIntervalMs = 1000.0f;
    int outputLine = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TTLEventGenerator);
};

#endif
