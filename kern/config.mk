# -*-Makefile-*-

#
# Kernel Building Parameters Setup
#
# This file defines all parameters controlling the building procedure of
# CertiKOS.
#

#
# Usage Examples:
#
# * Debug messages in following examples are messages produced by KERN_DEBUG()
#   and dprintf(). Other messages are not controlled by the debug-relevant
#   parameters in this file.
#
# 1. Enable printing debug messages,
#        DEBUG_MSG=1 make
#
# 2. Redirect all debug messages to the serial port (COM1),
#        DEBUG_MSG=1 SERIAL_DEBUG=1 make
#
# 3. Enable all debug messages,
#        DEBUG_MSG=1 DEBUG_ALL=1 make
#
# 4. Enable debug messages relevant to the virtualization module,
#        DEBUG_MSG=1 DEBUG_HVM=1 make
#
# 5. Enable debug messages relevant to the guest CPUID instructions,
#        DEBUG_MSG=1 DEBUG_GUEST_CPUID=1 make
#

#
# Add new building parameters
#
# Example:
#
# Suppose we want to control the building of a function foo() in the source.
#
# 1. Choose a fresh macro (e.g. ENABLE_FOO) to control foo() in the source,
#        #ifdef ENABLE_FOO
#        void foo() { ... }
#        #endif
#
# 2. Choose a fresh command line parameter name (e.g. ENABLE_FOO).
#
# 3. Add following lines in this file,
#        ifdef ENABLE_FOO
#        KERN_DEBUG_FLAGS += -DENABLE_FOO
#        endif
#
# Then you can use "ENABLE_FOO=1 make" to include the function foo() when
# building CertiKOS.
#

#
# Global debugging switches.
#

# If set, enable printing debug messages
ifneq "$(strip $(DEBUG_MSG) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_MSG
endif

# If set, print debug messages to serial port other than the screen
ifneq "$(strip $(SERIAL_DEBUG) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DSERIAL_DEBUG -DDEBUG_MSG
endif

#
# Debugging switches of the conventional kernel.
#

# If set, enable debugging the slab allocator
ifneq "$(strip $(DEBUG_SLAB) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_SLAB -DDEBUG_MSG
endif

# If set, enable debugging disk management
ifneq "$(strip $(DEBUG_DISK) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_DISK
endif

# If set, enable debugging AHCI driver
ifneq "$(strip $(DEBUG_AHCI) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_AHCI
endif

# If set, enable debugging processes
ifneq "$(strip $(DEBUG_PROC) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_PROC -DDEBUG_MSG
endif

# If set, enable debugging syscalls
ifneq "$(strip $(DEBUG_SYSCALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_SYSCALL -DDEBUG_MSG
endif

# If set, enable debugging channels
ifneq "$(strip $(DEBUG_CHANNEL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_CHANNEL -DDEBUG_MSG
endif

# If set, enable debugging scheduler
ifneq "$(strip $(DEBUG_SCHED) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_SCHED -DDEBUG_MSG
endif

# If set, enable debugging IPC
ifneq "$(strip $(DEBUG_IPC) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_IPC -DDEBUG_MSG
endif

#
# Debugging switches of the virtualization module.
#

# If set, enable the basic debugging messages for the virtualization
ifneq "$(strip $(DEBUG_HVM) $(DEBUG_HVM_ALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for VMX module
ifneq "$(strip $(DEBUG_VMX) $(DEBUG_HVM_ALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_VMX -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for the extended page table
ifneq "$(strip $(DEBUG_EPT) $(DEBUG_HVM_ALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_EPT -DDEBUG_VMX -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for SVM module
ifneq "$(strip $(DEBUG_SVM) $(DEBUG_HVM_ALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_SVM -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for VMEXITs
ifneq "$(strip $(DEBUG_VMEXIT) $(DEBUG_HVM_ALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_VMEXIT -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for guest interrupts
ifneq "$(strip $(DEBUG_GUEST_INTR) $(DEBUG_HVM_ALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_GUEST_INTR -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for guest event injections
ifneq "$(strip $(DEBUG_GUEST_INJECT) $(DEBUG_HVM_ALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_GUEST_INJECT -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for guest I/O ports
ifneq "$(strip $(DEBUG_GUEST_IOPORT) $(DEBUG_HVM_ALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_GUEST_IOPORT -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for guest MSRs
ifneq "$(strip $(DEBUG_GUEST_MSR) $(DEBUG_HVM_ALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_GUEST_MSR -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for guest page faults
ifneq "$(strip $(DEBUG_GUEST_PGFLT) $(DEBUG_HVM_ALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_GUEST_PGFLT -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for guest cpuid
ifneq "$(strip $(DEBUG_GUEST_CPUID) $(DEBUG_HVM_ALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_GUEST_CPUID -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for guest TSC
ifneq "$(strip $(DEBUG_GUEST_TSC) $(DEBUG_HVM_ALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_GUEST_TSC -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for hypercalls
ifneq "$(strip $(DEBUG_HYPERCALL) $(DEBUG_HVM_ALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_HYPERCALL -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable debugging virtual devices
ifneq "$(strip $(DEBUG_VDEV) $(DEBUG_HVM_ALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_VDEV -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable debugging virtualized PIC
ifneq "$(strip $(DEBUG_VPIC) $(DEBUG_VDEV) $(DEBUG_HVM_ALL) $(DEBUG_ALL))" ""
KERN_DEBUG_FLAGS	+= -DDEBUG_VPIC -DDEBUG_HVM -DDEBUG_MSG
endif

#
# Performace trace switches.
#

# If set, enable the basic trace of the virtualization module.
ifneq "$(strip $(TRACE_VIRT) $(TRACE_VIRT_ALL))" ""
KERN_DEBUG_FLAGS	+= -DTRACE_VIRT -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the basic trace of VMEXITs.
ifneq "$(strip $(TRACE_VMEXIT) $(TRACE_VIRT_ALL))" ""
KERN_DEBUG_FLAGS	+= -DTRACE_VMEXIT -DTRACE_VIRT -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the trace of handling guest interrupts.
ifneq "$(strip $(TRACE_GUEST_INTR) $(TRACE_VIRT_ALL))" ""
KERN_DEBUG_FLAGS	+= -DTRACE_GUEST_INTR -DTRACE_VIRT -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the trace of handling guest I/O ports.
ifneq "$(strip $(TRACE_GUEST_IOPORT) $(TRACE_VIRT_ALL))" ""
KERN_DEBUG_FLAGS	+= -DTRACE_GUEST_IOPORT -DTRACE_VIRT -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the trace of handling guest MSRs.
ifneq "$(strip $(TRACE_GUEST_MSR) $(TRACE_VIRT_ALL))" ""
KERN_DEBUG_FLAGS	+= -DTRACE_GUEST_MSR -DTRACE_VIRT -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the trace of handling guest cpuid.
ifneq "$(strip $(TRACE_GUEST_CPUID) $(TRACE_VIRT_ALL))" ""
KERN_DEBUG_FLAGS	+= -DTRACE_GUEST_CPUID -DTRACE_VIRT -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the trace of handling guest page fault.
ifneq "$(strip $(TRACE_GUEST_PGFLT) $(TRACE_VIRT_ALL))" ""
KERN_DEBUG_FLAGS	+= -DTRACE_GUEST_PGFLT -DTRACE_VIRT -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the trace of handling guest TSC.
ifneq "$(strip $(TRACE_GUEST_TSC) $(TRACE_VIRT_ALL))" ""
KERN_DEBUG_FLAGS	+= -DTRACE_GUEST_TSC -DTRACE_VIRT -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the trace of handling hypercalls.
ifneq "$(strip $(TRACE_HYPERCALL) $(TRACE_VIRT_ALL))" ""
KERN_DEBUG_FLAGS	+= -DTRACE_HYPERCALL -DTRACE_VIRT -DDEBUG_HVM -DDEBUG_MSG
endif

# If set, enable the test mode.
ifneq "$(TEST)" ""
KERN_DEBUG_FLAGS += -DTEST
endif
