#include <tick.h>
#include "infrared.h"
#include "../peripheral_layer/adc.h"

// ADC objects
static adc_t _adc_left;
static adc_t _adc_right;
static adc_t _adc_front_left;
static adc_t _adc_front_right;

void range_init(void) {
    /* ADC */
    // Left
    adc_init_t adc_init_obj;
    adc_init_obj.device = RECEIVER_ADC;
    adc_init_obj.channel = RECEIVER_L_CHANNEL;
    adc_init(&_adc_left, &adc_init_obj);
    adc_pin_set(RECEIVER_L_PORT, RECEIVER_L_PIN);

    // Right
    adc_init_obj.device = RECEIVER_ADC;
    adc_init_obj.channel = RECEIVER_R_CHANNEL;
    adc_init(&_adc_right, &adc_init_obj);
    adc_pin_set(RECEIVER_R_PORT, RECEIVER_R_PIN);

    // Front (actually FR)
    adc_init_obj.device = RECEIVER_ADC;
    adc_init_obj.channel = RECEIVER_FR_CHANNEL;
    adc_init(&_adc_front_right, &adc_init_obj);
    adc_pin_set(RECEIVER_FR_PORT, RECEIVER_FR_PIN);

    // Front (actually FL)
    adc_init_obj.device = RECEIVER_ADC;
    adc_init_obj.channel = RECEIVER_FL_CHANNEL;
    adc_init(&_adc_front_left, &adc_init_obj);
    adc_pin_set(RECEIVER_FL_PORT, RECEIVER_FL_PIN);

    /* Emitter */
    gpio_init_t GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init(EMITTER_L_PORT, EMITTER_L_PIN, &GPIO_InitStruct);
    gpio_init(EMITTER_R_PORT, EMITTER_R_PIN, &GPIO_InitStruct);
    gpio_init(EMITTER_FL_PORT, EMITTER_FL_PIN, &GPIO_InitStruct);
    gpio_init(EMITTER_FR_PORT, EMITTER_FR_PIN, &GPIO_InitStruct);

    gpio_set(EMITTER_L_PORT, EMITTER_L_PIN, GPIO_PIN_RESET);
    gpio_set(EMITTER_R_PORT, EMITTER_R_PIN, GPIO_PIN_RESET);
    gpio_set(EMITTER_FL_PORT, EMITTER_FL_PIN, GPIO_PIN_RESET);
    gpio_set(EMITTER_FR_PORT, EMITTER_FR_PIN, GPIO_PIN_RESET);
}

void range_get(struct range_data *out, enum range_ch ch) {
    if (NULL == out) {
        return;
    }
    // Get
    switch (ch) {
        case RANGE_CH_LEFT:
            gpio_set(EMITTER_L_PORT, EMITTER_L_PIN, GPIO_PIN_SET);
            delay_us(60);
            out->left = adc_measure(&_adc_left);
            gpio_set(EMITTER_L_PORT, EMITTER_L_PIN, GPIO_PIN_RESET);
            break;
        case RANGE_CH_RIGHT:
            gpio_set(EMITTER_R_PORT, EMITTER_R_PIN, GPIO_PIN_SET);
            delay_us(60);
            out->right = adc_measure(&_adc_right);
            gpio_set(EMITTER_R_PORT, EMITTER_R_PIN, GPIO_PIN_RESET);
            break;
        case RANGE_CH_FRONT:
            gpio_set(EMITTER_FL_PORT, EMITTER_FL_PIN, GPIO_PIN_SET);
            delay_us(60);
            out->front = adc_measure(&_adc_front_left);
            gpio_set(EMITTER_FL_PORT, EMITTER_FL_PIN, GPIO_PIN_RESET);
            gpio_set(EMITTER_FR_PORT, EMITTER_FR_PIN, GPIO_PIN_SET);
            delay_us(60);
            out->front_right = adc_measure(&_adc_front_right);
            gpio_set(EMITTER_FR_PORT, EMITTER_FR_PIN, GPIO_PIN_RESET);
            break;
        case RANGE_CH_ALL:
            range_get(out, RANGE_CH_LEFT);
            range_get(out, RANGE_CH_RIGHT);
            range_get(out, RANGE_CH_FRONT);
            break;
    }
}
