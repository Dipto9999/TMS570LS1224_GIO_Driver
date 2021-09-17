#include "gio.h"

/*
 * Initialize the GIO Module and The Ports to Normal GIO Mode.
 */
void gioInit(void) {
    /* Bring GIO Module Out of Reset. */

    /* Enable Bit 1 of GIO Global Control Register For GIO Module to Operate Normally. */
    gioREG->GCR0 = GIO_ENABLED;

    /* Disable Interrupts on Transition Edge of GIO Pins. */
    gioREG->ENACLR = MAX_VALUE;
    /* Set All GIO Interrupts as Low-Level Interrupts (Level B) Going to the Vectored Interrupt Manager (VIM). */
    gioREG->LVLCLR = MAX_VALUE;
    /* Ensure the GIO Interrupts Are Not Set As High-Level Interrupts (Level A) Going to the VIM. */
    gioREG->LVLSET = GIO_LVLSET_CONFIGVALUE;
    /* Configure Interrupts to Be Detected and Flags to Be Set on Falling Edge of GIO Pins. */
    gioREG->POL = GIO_POL_CONFIGVALUE;
    /* Set Interrupt Flag Register for GIO Pins to Indicate Transition Edge Has Occurred. */
    gioREG->FLG = MAX_VALUE;
    /* Ensure the Interrupts on the GIO Pins Are Disabled. */
    gioREG->ENASET = GIO_INTENASET_CONFIGVALUE;

    /* Initialize GIO Port A. */

    /* Set Output Values of GIO Pins to Logic Low. */
    gioPORTA->DOUT = GIO_PORTADOUT_CONFIGVALUE;
    /* Initialize All Pins as GIO Input */
    gioPORTA->DIR = GIO_PORTADIR_CONFIGVALUE;
    /* Disable the Open Drain Capability of GIO Pins To Configure in Push/Pull Mode (Normal GIO Mode). */
    gioPORTA->PDR = GIO_PORTAPDR_CONFIGVALUE;
    /* Select the Pull Down Functionality for GIO Pins. */
    gioPORTA->PSL = GIO_PORTAPSL_CONFIGVALUE;
    /* Enable the Pull Functionality of the GIO Pins. */
    gioPORTA->PULDIS = GIO_PORTAPULDIS_CONFIGVALUE;

    /* Initialize GIO Port B. */

    /* Set Output Values of GIO Pins to Logic Low. */
    gioPORTB->DOUT = GIO_PORTBDOUT_CONFIGVALUE;
    /* Initialize All Pins as GIO Input */
    gioPORTB->DIR = GIO_PORTBDIR_CONFIGVALUE;
    /* Disable the Open Drain Capability of GIO Pins To Configure in Push/Pull Mode (Normal GIO Mode). */
    gioPORTB->PDR = GIO_PORTBPDR_CONFIGVALUE;
    /* Select the Pull Down Functionality for GIO Pins. */
    gioPORTB->PSL = GIO_PORTBPSL_CONFIGVALUE;
    /* Enable the Pull Functionality of the GIO Pins. */
    gioPORTB->PULDIS = GIO_PORTBPULDIS_CONFIGVALUE;
}

/*
 * Set the Direction of a GIO Pin.
 *
 * 0 = input
 * 1 = output
 */
void gioSetDirection(gioPORT_t *port, uint32_t pin, uint32_t dir) {
    if (dir == GIO_ENABLED) port->DIR |= dir << pin;
    else port->DIR &= MAX_VALUE - ~dir << pin;
}

/*
 * Turn a GIO Pin On or Off.
 *
 * 0 = OFF
 * 1 = ON
 */
void gioWrite(gioPORT_t *port, uint32_t pin, uint32_t value, uint32_t write_config) {
    switch (write_config) {
        /* Turn the GIO Pin Off By Writing to Data Clear Register. */
        case DCLR_GIO_DISABLE:
            if (value == GIO_DISABLED) port->DCLR = GIO_ENABLED << pin;
            else port->DOUT |= value << pin; /* In Case Configurations Are Not Aligned. */
            break;
        /* Turn the GIO Pin On By Writing to Data Output Register. */
		case DOUT_GIO_ENABLE:
            if (value == GIO_ENABLED) port->DOUT |= value << pin;
            else port->DCLR |= value << pin; /* In Case Configurations Are Not Aligned. */
            break;
        /* Turn the GIO Pin Off By Writing to Data Output Register. */
        case DOUT_GIO_DISABLE:
            if (value == GIO_DISABLED) port->DOUT &= MAX_VALUE - (GIO_ENABLED << pin);
            else port->DSET |= value << pin; /* In Case Configurations Are Not Aligned. */
            break;
        /* Turn the GIO Pin On By Writing to Data Set Register. */
        case DSET_GIO_ENABLE:
            if (value == GIO_ENABLED) port->DSET |= value << pin;
            else port->DOUT &= MAX_VALUE - (GIO_ENABLED << pin); /* In Case Configurations Are Not Aligned. */
            break;
    }

    return;
}

/*
 * Read the Value of a GIO Pin.
 */
uint32_t gioRead(gioPORT_t *port, uint32_t pin) {
    return (port->DIN >> pin & GIO_ENABLED);
}

/*
 * Toggle a GIO Pin.
 */
void gioToggle(gioPORT_t *port, uint32_t pin, uint32_t config) {
    gioWrite(port, pin, gioRead(port, pin) ^ GIO_ENABLED, config);
}
