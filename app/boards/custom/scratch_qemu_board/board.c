#include <zephyr/init.h>
#include <zephyr/sys/printk.h>

static int board_pre_main_init(void)
{
    /* Force an absolute immediate flush to the raw UART hardware */
    printk("Board Initialized\n");
    return 0;
}

/* Move to PRE_KERNEL_1 with maximum priority (0) to run first */
SYS_INIT(board_pre_main_init, PRE_KERNEL_1, 0);
