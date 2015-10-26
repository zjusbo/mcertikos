#ifndef _KERN_DEV_LAPIC_H_
#define _KERN_DEV_LAPIC_H_

#ifdef _KERN_

#include <lib/types.h>

#define LAPIC_ID		(0x0020/4)   // ID
#define LAPIC_VER		(0x0030/4)   // Version
#define LAPIC_TPR		(0x0080/4)   // Task Priority
#define LAPIC_APR		(0x0090/4)   // Arbitration Priority
#define LAPIC_PPR		(0x00A0/4)   // Processor Priority
#define LAPIC_EOI		(0x00B0/4)   // EOI
#define LAPIC_LDR		(0x00D0/4)   // Logical Destination
#define LAPIC_DFR		(0x000E0/4)  // Destination Format
# define LAPIC_DFR_FLAT_MODE	(0xF << 28)  // Flat Mode
#define LAPIC_SVR		(0x00F0/4)   // Spurious Interrupt Vector
# define LAPIC_SVR_ENABLE	0x00000100   // Unit Enable
#define LAPIC_ISR		(0x0100/4)   //ISR
#define LAPIC_IRR		(0x0200/4)   //IRR
#define LAPIC_ESR		(0x0280/4)   // Error Status
#define LAPIC_ICRLO		(0x0300/4)   // Interrupt Command
# define LAPIC_ICRLO_VECTOR	0xFF         // Vector Mask
# define LAPIC_ICRLO_FIXED	0x00000000   // Fixed
# define LAPIC_ICRLO_LOWEST	0x00000100   // Lowest Priority
# define LAPIC_ICRLO_SMI	0x00000200   // SMI
# define LAPIC_ICRLO_NMI	0x00000400   // NMI
# define LAPIC_ICRLO_INIT       0x00000500   // INIT/RESET
# define LAPIC_ICRLO_STARTUP    0x00000600   // Startup IPI
# define LAPIC_ICRLO_LOGIC	0x00000800   // Logical Mode
# define LAPIC_ICRLO_DELIVS     0x00001000   // Delivery status
# define LAPIC_ICRLO_ASSERT     0x00004000   // Assert interrupt (vs deassert)
# define LAPIC_ICRLO_LEVEL      0x00008000   // Level triggered
# define LAPIC_ICRLO_NOBCAST	0x00000000   // No Shorthand
# define LAPIC_ICRLO_SELF       0x00040000   // Send to self
# define LAPIC_ICRLO_BCAST      0x00080000   // Send to all APICs, including self.
# define LAPIC_ICRLO_OTHERS	0x000C0000   // Send to all other APICs
#define LAPIC_ICRHI		(0x0310/4)   // Interrupt Command [63:32]
# define LAPIC_ICRHI_DEST_MASK	0xFF000000   // Destination Mask
# define LAPIC_ICRHI_DEST_SHIFT	24           // Destination Shift
#define LAPIC_TIMER		(0x0320/4)   // Local Vector Table 0 (TIMER)
# define LAPIC_TIMER_MASKED	0x00010000
# define LAPIC_TIMER_X1		0x0000000B   // divide counts by 1
# define LAPIC_TIMER_PERIODIC   0x00020000   // Periodic
#define LAPIC_PCINT		(0x0340/4)   // Performance Counter LVT
# define APIC_LVTT_VECTOR	0x000000ff
# define APIC_LVTT_DS		0x00001000
# define APIC_LVTT_M		0x00010000
# define APIC_LVTT_TM		0x00020000
#  define APIC_LVTT_TM_ONE_SHOT	0x00000000
#  define APIC_LVTT_TM_PERIODIC	0x00020000
#define LAPIC_LINT0		(0x0350/4)   // Local Vector Table 1 (LINT0)
#define LAPIC_LINT1		(0x0360/4)   // Local Vector Table 2 (LINT1)
# define LAPIC_LINT_MASKED     	0x00010000
#define LAPIC_ERROR		(0x0370/4)   // Local Vector Table 3 (ERROR)
# define LAPIC_ERROR_MASKED     0x00010000   // Interrupt masked
#define LAPIC_TICR		(0x0380/4)   // Timer Initial Count
#define LAPIC_TCCR		(0x0390/4)   // Timer Current Count
#define LAPIC_TDCR		(0x03E0/4)   // Timer Divide Configuration
# define APIC_TDCR_2		0x00
# define APIC_TDCR_4		0x01
# define APIC_TDCR_8		0x02
# define APIC_TDCR_16		0x03
# define APIC_TDCR_32		0x08
# define APIC_TDCR_64		0x09
# define APIC_TDCR_128		0x0a
# define APIC_TDCR_1		0x0b

/* fields in ICR_LOW */
#define APIC_VECTOR_MASK	0x000000ff

#define APIC_DELMODE_MASK	0x00000700
# define APIC_DELMODE_FIXED	0x00000000
# define APIC_DELMODE_LOWPRIO	0x00000100
# define APIC_DELMODE_SMI	0x00000200
# define APIC_DELMODE_RR	0x00000300
# define APIC_DELMODE_NMI	0x00000400
# define APIC_DELMODE_INIT	0x00000500
# define APIC_DELMODE_STARTUP	0x00000600
# define APIC_DELMODE_RESV	0x00000700

#define APIC_DESTMODE_MASK	0x00000800
# define APIC_DESTMODE_PHY	0x00000000
# define APIC_DESTMODE_LOG	0x00000800

#define APIC_DELSTAT_MASK	0x00001000
# define APIC_DELSTAT_IDLE	0x00000000
# define APIC_DELSTAT_PEND	0x00001000

#define APIC_RESV1_MASK		0x00002000

#define APIC_LEVEL_MASK		0x00004000
# define APIC_LEVEL_DEASSERT	0x00000000
# define APIC_LEVEL_ASSERT	0x00004000

#define APIC_TRIGMOD_MASK	0x00008000
# define APIC_TRIGMOD_EDGE	0x00000000
# define APIC_TRIGMOD_LEVEL	0x00008000

#define APIC_RRSTAT_MASK	0x00030000
# define APIC_RRSTAT_INVALID	0x00000000
# define APIC_RRSTAT_INPROG	0x00010000
# define APIC_RRSTAT_VALID	0x00020000
# define APIC_RRSTAT_RESV	0x00030000

#define APIC_DEST_MASK		0x000c0000
# define APIC_DEST_DESTFLD	0x00000000
# define APIC_DEST_SELF		0x00040000
# define APIC_DEST_ALLISELF	0x00080000
# define APIC_DEST_ALLESELF	0x000c0000

#define APIC_RESV2_MASK		0xfff00000

#define	APIC_ICRLO_RESV_MASK	(APIC_RESV1_MASK | APIC_RESV2_MASK)

#define LAPIC_TIMER_INTR_FREQ	1000

typedef uintptr_t		lapic_t;

typedef uint8_t			lapicid_t;

void lapic_register(uintptr_t);
void lapic_init(void);
void lapic_eoi(void);
void lapic_startcpu(lapicid_t, uintptr_t);

uint32_t lapic_read_debug(int);

void lapic_send_ipi(lapicid_t lapicid, uint8_t vector,
		    uint32_t deliver_mode, uint32_t shorthand_mode);

#endif /* _KERN_ */

#endif /* !_SYS_DEV_LAPIC_H_ */
