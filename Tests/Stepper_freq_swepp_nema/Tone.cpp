/*

#include <Arduino.h>
#include "esp32-hal-ledc.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

static TaskHandle_t _tone_task = NULL;
static QueueHandle_t _tone_queue = NULL;
static uint8_t _channel = 0;

typedef enum{
  TONE_START,
  TONE_END,
  TONE_SET_CHANNEL
} tone_cmd_t;

typedef struct{
  tone_cmd_t tone_cmd;
  uint8_t pin;
  unsigned int frequency;
  unsigned long duration;
  uint8_t channel;
} tone_msg_t;

static void tone_task(void*){
  tone_msg_t tone_msg;
  while(1){
    xQueueReceive(_tone_queue, &tone_msg, portMAX_DELAY);
    switch(tone_msg.tone_cmd){
      case TONE_START:

        ledcAttachPin(tone_msg.pin, _channel);
        ledcWriteTone(_channel, tone_msg.frequency);

        if(tone_msg.duration){
          delay(tone_msg.duration);
          ledcDetachPin(tone_msg.pin);
          ledcWriteTone(_channel, 0);
        }
        break;

      case TONE_END:

        ledcDetachPin(tone_msg.pin);
        ledcWriteTone(_channel, 0);
        break;

      case TONE_SET_CHANNEL:

        _channel = tone_msg.channel;
        break;

      default: ; // do nothing
    } // switch
  } // infinite loop
}

static int tone_init(){
  if(_tone_queue == NULL){

    _tone_queue = xQueueCreate(128, sizeof(tone_msg_t));
    if(_tone_queue == NULL){

      return 0; // ERR
    }

  }

  if(_tone_task == NULL){

    xTaskCreate(
      tone_task, // Function to implement the task
      "toneTask", // Name of the task
      3500,  // Stack size in words
      NULL,  // Task input parameter
      1,  // Priority of the task
      &_tone_task  // Task handle.
      );
    if(_tone_task == NULL){

      return 0; // ERR
    }

  }
  return 1; // OK
}

void setToneChannel(uint8_t channel){

  if(tone_init()){
    tone_msg_t tone_msg = {
      .tone_cmd = TONE_SET_CHANNEL,
      .pin = 0, // Ignored
      .frequency = 0, // Ignored
      .duration = 0, // Ignored
      .channel = channel
    };
    xQueueSend(_tone_queue, &tone_msg, portMAX_DELAY);
  }
}

void noTone(uint8_t _pin){

  if(tone_init()){
    tone_msg_t tone_msg = {
      .tone_cmd = TONE_END,
      .pin = _pin,
      .frequency = 0, // Ignored
      .duration = 0, // Ignored
      .channel = 0 // Ignored
    };
    xQueueSend(_tone_queue, &tone_msg, portMAX_DELAY);
  }
}

// parameters:
// _pin - pin number which will output the signal
// frequency - PWM frequency in Hz
// duration - time in ms - how long will the signal be outputted.
//   If not provided, or 0 you must manually call noTone to end output
void tone(uint8_t _pin, unsigned int frequency, unsigned long duration){

  if(tone_init()){
    tone_msg_t tone_msg = {
      .tone_cmd = TONE_START,
      .pin = _pin,
      .frequency = frequency,
      .duration = duration,
      .channel = 0 // Ignored
    };
    xQueueSend(_tone_queue, &tone_msg, portMAX_DELAY);
  }
}
*/