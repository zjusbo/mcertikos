# If set, enable debugging virtualized keyboard
ifneq "$(strip $(DEBUG_VKBD) $(DEBUG_ALL) $(DEBUG_VMM_ALL))" ""
USER_CFLAGS	+= -DDEBUG_VKBD
endif

# If set, enable debugging virtualized PIT
ifneq "$(strip $(DEBUG_VPIT) $(DEBUG_ALL) $(DEBUG_VMM_ALL))" ""
USER_CFLAGS	+= -DDEBUG_VPIT
endif

# If set, enable debugging virtualized PCI
ifneq "$(strip $(DEBUG_VPCI) $(DEBUG_ALL) $(DEBUG_VMM_ALL))" ""
USER_CFLAGS	+= -DDEBUG_VPCI
endif

# If set, enable common virtio device debug
ifneq "$(strip $(DEBUG_VIRTIO) $(DEBUG_ALL) $(DEBUG_VMM_ALL))" ""
USER_CFLAGS	+= -DDEBUG_VIRTIO
endif

# If set, enable debugging virtio block device
ifneq "$(strip $(DEBUG_VIRTIO_BLK) $(DEBUG_ALL) $(DEBUG_VMM_ALL))" ""
USER_CFLAGS	+= -DDEBUG_VIRTIO -DDEBUG_VIRTIO_BLK
endif

# If set, enable debugging virtulaized NVRAM
ifneq "$(strip $(DEBUG_VNVRAM) $(DEBUG_ALL) $(DEBUG_VMM_ALL))" ""
USER_CFLAGS	+= -DDEBUG_VNVRAM
endif

# If set, both CertiKOS and the guest Linux are on the same CF card
ifdef ENABLE_BOOT_CF
USER_CFLAGS	+= -DENABLE_BOOT_CF
endif

# If set, enable the basic debugging messages for the virtualization
ifneq "$(strip $(DEBUG_VMM) $(DEBUG_VMM_ALL) $(DEBUG_ALL))" ""
USER_CFLAGS	+= -DDEBUG_VMM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for VMX module
ifneq "$(strip $(DEBUG_VMX) $(DEBUG_VMM) $(DEBUG_ALL))" ""
USER_CFLAGS	+= -DDEBUG_VMX -DDEBUG_VMM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for the extended page table
ifneq "$(strip $(DEBUG_EPT) $(DEBUG_VMM) $(DEBUG_ALL))" ""
USER_CFLAGS	+= -DDEBUG_EPT -DDEBUG_VMX -DDEBUG_VMM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for SVM module
ifneq "$(strip $(DEBUG_SVM) $(DEBUG_VMM) $(DEBUG_ALL))" ""
USER_CFLAGS	+= -DDEBUG_SVM -DDEBUG_VMM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for VMEXITs
ifneq "$(strip $(DEBUG_VMEXIT) $(DEBUG_VMM) $(DEBUG_ALL))" ""
USER_CFLAGS	+= -DDEBUG_VMEXIT -DDEBUG_VMM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for guest interrupts
ifneq "$(strip $(DEBUG_GUEST_INTR) $(DEBUG_VMM) $(DEBUG_ALL))" ""
USER_CFLAGS	+= -DDEBUG_GUEST_INTR -DDEBUG_VMM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for guest event injections
ifneq "$(strip $(DEBUG_GUEST_INJECT) $(DEBUG_VMM) $(DEBUG_ALL))" ""
USER_CFLAGS	+= -DDEBUG_GUEST_INJECT -DDEBUG_VMM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for guest I/O ports
ifneq "$(strip $(DEBUG_GUEST_IOPORT) $(DEBUG_VMM) $(DEBUG_ALL))" ""
USER_CFLAGS	+= -DDEBUG_GUEST_IOPORT -DDEBUG_VMM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for guest MSRs
ifneq "$(strip $(DEBUG_GUEST_MSR) $(DEBUG_VMM) $(DEBUG_ALL))" ""
USER_CFLAGS	+= -DDEBUG_GUEST_MSR -DDEBUG_VMM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for guest page faults
ifneq "$(strip $(DEBUG_GUEST_PGFLT) $(DEBUG_VMM) $(DEBUG_ALL))" ""
USER_CFLAGS	+= -DDEBUG_GUEST_PGFLT -DDEBUG_VMM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for guest cpuid
ifneq "$(strip $(DEBUG_GUEST_CPUID) $(DEBUG_VMM) $(DEBUG_ALL))" ""
USER_CFLAGS	+= -DDEBUG_GUEST_CPUID -DDEBUG_VMM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for guest TSC
ifneq "$(strip $(DEBUG_GUEST_TSC) $(DEBUG_VMM) $(DEBUG_ALL))" ""
USER_CFLAGS	+= -DDEBUG_GUEST_TSC -DDEBUG_VMM -DDEBUG_MSG
endif

# If set, enable the basic debugging messages for hypercalls
ifneq "$(strip $(DEBUG_HYPERCALL) $(DEBUG_VMM) $(DEBUG_ALL))" ""
USER_CFLAGS	+= -DDEBUG_HYPERCALL -DDEBUG_VMM -DDEBUG_MSG
endif

# If set, enable debugging virtual devices
ifneq "$(strip $(DEBUG_VDEV) $(DEBUG_VMM_ALL) $(DEBUG_ALL))" ""
USER_CFLAGS	+= -DDEBUG_VDEV -DDEBUG_VMM -DDEBUG_MSG
endif

# If set, enable debugging virtualized PIC
ifneq "$(strip $(DEBUG_VPIC) $(DEBUG_VDEV) $(DEBUG_VMM_ALL) $(DEBUG_ALL))" ""
USER_CFLAGS	+= -DDEBUG_VPIC -DDEBUG_VMM -DDEBUG_MSG
endif

# If set, enable application setting vmm
ifneq "$(strip $(CONFIG_APP_VMM))" ""
USER_CFLAGS	+= -DCONFIG_APP_VMM
endif

# If set, enable application setting user proc
ifneq "$(strip $(CONFIG_APP_USER_PROC))" ""
USER_CFLAGS	+= -DCONFIG_APP_USER_PROC
endif

# If set, enable application setting ring zero proc
ifneq "$(strip $(CONFIG_APP_RING0_PROC))" ""
USER_CFLAGS	+= -DCONFIG_APP_RING0_PROC
endif

# If set, enable application setting profiling
ifneq "$(strip $(CONFIG_APP_USER_PROFILE))" ""
USER_CFLAGS	+= -DCONFIG_APP_USER_PROFILE
endif




