.syntax unified
.cpu cortex-m4
.fpu fpv4-sp-d16
.thumb

.global g_pfnVectors
.global Reset_Handler
.global Default_Handler
.global HardFault_Handler
.global _estack

.weak NMI_Handler
.thumb_set NMI_Handler,Default_Handler
.weak MemManage_Handler
.thumb_set MemManage_Handler,Default_Handler
.weak BusFault_Handler
.thumb_set BusFault_Handler,Default_Handler
.weak UsageFault_Handler
.thumb_set UsageFault_Handler,Default_Handler
.weak SVC_Handler
.thumb_set SVC_Handler,Default_Handler
.weak DebugMon_Handler
.thumb_set DebugMon_Handler,Default_Handler
.weak PendSV_Handler
.thumb_set PendSV_Handler,Default_Handler
.weak SysTick_Handler
.thumb_set SysTick_Handler,Default_Handler

.type HardFault_Handler, %function

.section .isr_vector, "a", %progbits
g_pfnVectors:
  .word _estack
  .word Reset_Handler + 1

  /* Exception handlers */
  .word Default_Handler + 1 /* NMI */
  .word HardFault_Handler + 1
  .word Default_Handler + 1 /* MemManage */
  .word Default_Handler + 1 /* BusFault */
  .word Default_Handler + 1 /* UsageFault */
  .word 0
  .word 0
  .word 0
  .word 0
  .word Default_Handler + 1 /* SVC */
  .word Default_Handler + 1 /* DebugMon */
  .word 0
  .word PendSV_Handler + 1 /* PendSV */
  .word SysTick_Handler + 1 /* SysTick */

.section .text.Reset_Handler, "ax", %progbits
.thumb_func
Reset_Handler:
  ldr r0, =0xE000ED08     /* SCB->VTOR */
  ldr r1, =0x08000000
  str r1, [r0]

  ldr r0, =_sidata
  ldr r1, =_sdata
  ldr r2, =_edata
1:
  cmp r1, r2
  bcc 2f 
  b   3f
2:
  ldr r3, [r0], #4
  str r3, [r1], #4
  b   1b
3:
  ldr r0, =_sbss
  ldr r1, =_ebss
  movs r2, #0
4:
  cmp r0, r1
  bcc 5f
  b   6f
5:
  str r2, [r0], #4
  b   4b
6:
  bl  main
7:
  b   7b

.section .text.HardFault_Handler, "ax", %progbits
; .thumb_func
HardFault_Handler:
  b HardFault_Handler

.section .text.Default_Handler, "ax", %progbits
; .thumb_func
Default_Handler:
  b Default_Handler
