
// the console cant be defined now, they are already compiled
// into the kernel... Until fixed. make sure that both SYS_CON and
// USER_CON. are directed to the usart. We want the usb free for
// the panel system.
#define SYS_CONSOLE_DEV	"usart0"
#define USER_CONSOLE_DEV "usart0"

#include_next "config.h"


#if defined(BOARD_MB997C)
#define OBD1_PIN GPIO_PIN(GPIO_PC,5)
#elif defined(BOARD_BLACKPILL)
#define OBD1_PIN GPIO_PIN(GPIO_PB,10)
#else
#error "Board not defined"

#endif
