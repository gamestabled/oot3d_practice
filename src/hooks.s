.arm

.section .patch_main
.global main_patch

main_patch:
    b hook_into_main    

.section .patch_load
.global load_patch

load_patch:
    b hook_into_area_load