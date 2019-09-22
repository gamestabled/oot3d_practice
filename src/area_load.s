.arm
.text

.global hook_into_main
hook_into_main:
    push {r0-r12, lr}
    bl advance_main
    pop {r0-r12, lr}
    pop {r4- r8, pc}


.global hook_into_area_load
hook_into_area_load:
    push {r0-r12, lr}
    bl area_load_main
    pop {r0-r12, lr}
    b 0x2e37d4

.global svcSleepThread
svcSleepThread:
    svc 0x0a
    bx lr

