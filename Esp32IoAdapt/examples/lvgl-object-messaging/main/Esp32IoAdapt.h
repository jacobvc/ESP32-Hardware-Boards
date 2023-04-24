// NO CONNECTION == GPIO_NUM_NC
#define ACD_CHAN_NC ((adc_channel_t)-1)

/* Connector definitions
 *
 * Naming prefix for this Esp32 I/O Adapter Board is IA_
 *
 * Each connector contains GROUND, optional 3.3 Volt supply and one or more signals
 * 
 * Subsequent pins are "Signal" GPIO's.
 * A full featured signal GPIO is referred to as "S", and an input only signal GPIO is 
 * referred to as "I". 
 * 
 * A connector is named by its signals and instance, e.g. GVS1, 
 * GVI1, GVI2, GVSS1, etc. This name is on the board silkscreen
 * 
 * Each signal GPIO has a pin definition <connector>_PIN, and each analog
 * capable GPIO has a channel definition <connector>_CHAN.
 * 
 * When a connector has a single GPIO, the definitions are literal defines. When a
 * connector has more than one signal, the definitions are plural and represent 
 * arrays (<connector>_PINS[], <connector>_CHANS[]).
 * 
 * For example, a thumb joystick connected to connector GVSSS1 would initialize the 
 * analog channels to GVIIS1_CHAN[0] and GVIIS1_CHAN[1], and the button input to 
 * GVIIS1_PIN[2].
 * 
 */
// GS 1
#define IA_GS1_PIN                GPIO_NUM_27
// GVS 1
#define IA_GVS1_PIN               GPIO_NUM_14
// GVI 1
#define IA_GVI1_PIN               GPIO_NUM_36
#define IA_GVI1_CHAN              ADC_CHANNEL_0
// GVI 2
#define IA_GVI2_PIN               GPIO_NUM_39
#define IA_GVI2_CHAN              ADC_CHANNEL_3
// GVSSI 1
#define IA_GVSS1_PINS  (gpio_num_t[])    {GPIO_NUM_12, GPIO_NUM_13}
// GVSSS 1
#define IA_GVSSS1_PINS (gpio_num_t[])    {GPIO_NUM_33, GPIO_NUM_32, GPIO_NUM_26}
#define IA_GVSSS1_CHANS (adc_channel_t[]) {ADC_CHANNEL_5, ADC_CHANNEL_4, ACD_CHAN_NC}
// GVIIS 1
#define IA_GVIIS1_PINS (gpio_num_t[])    {GPIO_NUM_35, GPIO_NUM_34, GPIO_NUM_25}
#define IA_GVIIS1_CHANS (adc_channel_t[]) {ADC_CHANNEL_7, ADC_CHANNEL_6, ACD_CHAN_NC}
