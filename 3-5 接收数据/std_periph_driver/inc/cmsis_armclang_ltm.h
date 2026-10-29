/**************************************************************************//**
 * @file     cmsis_armclang_ltm.h
 * @brief    CMSIS compiler ARMCLANG (Arm Compiler 6 LTM) 兼容头文件
 * @version  V5.1.0（兼容层）
 * @date     2026-10-24
 ******************************************************************************/
/*
 * 说明：
 * 工程内的 core_cm3.h / cmsis_compiler.h 属于 CMSIS 5.1，但 inc 目录缺少 AC6
 * 对应的 cmsis_armclang_ltm.h / cmsis_armclang.h，导致 VS Code IntelliSense 报
 * “无法打开源文件”。
 * 本文件在 5.0.1 版 cmsis_armclang.h 的基础上补齐 CMSIS 5.1 新增的通用宏，
 * 仅供 VS Code（armclang）做 IntelliSense 解析使用。
 */

#ifndef __CMSIS_ARMCLANG_LTM_H
#define __CMSIS_ARMCLANG_LTM_H

#include "cmsis_armclang.h"

#ifndef __STATIC_FORCEINLINE
  #define __STATIC_FORCEINLINE                   __attribute__((always_inline)) static __inline
#endif

#ifndef __PACKED_UNION
  #define __PACKED_UNION                         union __attribute__((packed, aligned(1)))
#endif

#ifndef __ALIGNED
  #define __ALIGNED(x)                           __attribute__((aligned(x)))
#endif

#ifndef __RESTRICT
  #define __RESTRICT                             __restrict
#endif

#ifndef __COMPILER_BARRIER
  #define __COMPILER_BARRIER()                   __ASM volatile ("":::"memory")
#endif

#ifndef __UNALIGNED_UINT16_WRITE
  __PACKED_STRUCT T_UINT16_WRITE { uint16_t v; };
  #define __UNALIGNED_UINT16_WRITE(addr, val)    (void)((((struct T_UINT16_WRITE *)(void *)(addr))->v) = (val))
#endif

#ifndef __UNALIGNED_UINT16_READ
  __PACKED_STRUCT T_UINT16_READ { uint16_t v; };
  #define __UNALIGNED_UINT16_READ(addr)          (((const struct T_UINT16_READ *)(const void *)(addr))->v)
#endif

#ifndef __UNALIGNED_UINT32_WRITE
  __PACKED_STRUCT T_UINT32_WRITE { uint32_t v; };
  #define __UNALIGNED_UINT32_WRITE(addr, val)    (void)((((struct T_UINT32_WRITE *)(void *)(addr))->v) = (val))
#endif

#ifndef __UNALIGNED_UINT32_READ
  __PACKED_STRUCT T_UINT32_READ { uint32_t v; };
  #define __UNALIGNED_UINT32_READ(addr)          (((const struct T_UINT32_READ *)(const void *)(addr))->v)
#endif

#endif /* __CMSIS_ARMCLANG_LTM_H */
