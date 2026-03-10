#include "TTLEventGenerator.h"
#include "TTLEventGeneratorEditor.h"

TTLEventGenerator::TTLEventGenerator() : GenericProcessor("TTL Event Generator") {}
TTLEventGenerator::~TTLEventGenerator() {}

void TTLEventGenerator::registerParameters()
{
    // 가이드: 파라미터 등록
    addFloatParameter(Parameter::PROCESSOR_SCOPE, "interval", "Interval", "ms", 1000.0f, 0.0f, 5000.0f, 50.0f);
    addTtlLineParameter(Parameter::STREAM_SCOPE, "output_line", "Output line", "Output line for generated TTL events");
    addNotificationParameter(Parameter::PROCESSOR_SCOPE, "manual_trigger", "Trigger", "Triggers a TTL event", false);
}

void TTLEventGenerator::parameterValueChanged(Parameter* param)
{
    if (param->getName().equalsIgnoreCase("manual_trigger")) {
        shouldTriggerEvent = true;
    } else if (param->getName().equalsIgnoreCase("interval")) {
        eventIntervalMs = (float)param->getValue();
    } else if (param->getName().equalsIgnoreCase("output_line")) {
        outputLine = (int)param->getValue();
    }
}

void TTLEventGenerator::updateSettings()
{
    EventChannel::Settings settings{ EventChannel::Type::TTL, "TTL Event Generator Output", "Default TTL event channel", "ttl.events", dataStreams[0] };
    ttlChannel = new EventChannel(settings);
    eventChannels.add(ttlChannel);
    ttlChannel->addProcessor(this);
}

bool TTLEventGenerator::startAcquisition()
{
    counter = 0;
    state = false;
    return true;
}

void TTLEventGenerator::process(AudioBuffer<float>& buffer)
{
    for (auto stream : getDataStreams())
    {
        if (stream == getDataStreams()[0])
        {
            int totalSamples = getNumSamplesInBlock(stream->getStreamId());
            uint64 startSampleForBlock = getFirstSampleNumberForBlock(stream->getStreamId());
            int eventIntervalInSamples = (int)(stream->getSampleRate() * eventIntervalMs / 2000.0);

            if (shouldTriggerEvent) {
                TTLEventPtr eventPtr = TTLEvent::createTTLEvent(ttlChannel, startSampleForBlock, outputLine, true);
                addEvent(eventPtr, 0);
                shouldTriggerEvent = false;
                eventWasTriggered = true;
                triggeredEventCounter = 0;
            }

            for (int i = 0; i < totalSamples; i++) {
                counter++;
                if (eventWasTriggered) triggeredEventCounter++;

                if (triggeredEventCounter == eventIntervalInSamples) {
                    TTLEventPtr eventPtr = TTLEvent::createTTLEvent(ttlChannel, startSampleForBlock + i, outputLine, false);
                    addEvent(eventPtr, i);
                    eventWasTriggered = false;
                    triggeredEventCounter = 0;
                }

                if (counter == eventIntervalInSamples && eventIntervalMs > 0) {
                    state = !state;
                    TTLEventPtr eventPtr = TTLEvent::createTTLEvent(ttlChannel, startSampleForBlock + i, outputLine, state);
                    addEvent(eventPtr, i);
                    counter = 0;
                }
            }
        }
    }
}

AudioProcessorEditor* TTLEventGenerator::createEditor() { return new TTLEventGeneratorEditor(this); }
