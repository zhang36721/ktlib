/**
 * @file kt_error.h
 * @author kento
 * @brief 错误码定义
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 * 
 * @attention 本文件定义了ktlib库中使用的统一错误码
 */

#ifndef KT_ERROR_H
#define KT_ERROR_H

#include "kt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================
 *                         错误码定义
 *============================================================================*/

/**
 * @brief 错误码基础值
 * @note 所有错误码基于此值偏移
 */
#define KT_ERR_BASE             0

/**
 * @defgroup KT_Error_Codes 错误码定义
 * @{
 */

/* 成功状态 */
#define KT_OK                   ((kt_status_t)0)        /**< 成功 */
#define KT_SUCCESS              KT_OK                     /**< 成功（别名） */

/* 通用错误 */
#define KT_ERROR                ((kt_status_t)-1)       /**< 通用错误 */
#define KT_ERR_FAIL             ((kt_status_t)-1)       /**< 操作失败 */
#define KT_ERR_NO_MEM           ((kt_status_t)-2)       /**< 内存不足 */
#define KT_ERR_NO_RESOURCE      ((kt_status_t)-3)       /**< 资源不足 */
#define KT_ERR_TIMEOUT          ((kt_status_t)-4)       /**< 超时 */
#define KT_ERR_BUSY             ((kt_status_t)-5)       /**< 忙 */
#define KT_ERR_OVERFLOW         ((kt_status_t)-6)       /**< 溢出 */
#define KT_ERR_UNDERFLOW        ((kt_status_t)-7)       /**< 下溢 */

/* 参数错误 */
#define KT_ERR_INVALID_PARAM    ((kt_status_t)-10)      /**< 无效参数 */
#define KT_ERR_NULL_PTR         ((kt_status_t)-11)      /**< 空指针 */
#define KT_ERR_OUT_OF_RANGE     ((kt_status_t)-12)      /**< 超出范围 */
#define KT_ERR_NOT_SUPPORTED    ((kt_status_t)-13)      /**< 不支持 */
#define KT_ERR_INVALID_STATE    ((kt_status_t)-14)      /**< 无效状态 */
#define KT_ERR_INVALID_HANDLE   ((kt_status_t)-15)      /**< 无效句柄 */

/* 初始化错误 */
#define KT_ERR_NOT_INIT         ((kt_status_t)-20)      /**< 未初始化 */
#define KT_ERR_ALREADY_INIT     ((kt_status_t)-21)      /**< 已初始化 */
#define KT_ERR_INIT_FAILED      ((kt_status_t)-22)      /**< 初始化失败 */

/* 硬件相关错误 */
#define KT_ERR_HW_FAIL          ((kt_status_t)-30)      /**< 硬件故障 */
#define KT_ERR_IO               ((kt_status_t)-31)      /**< IO错误 */
#define KT_ERR_COMM             ((kt_status_t)-32)      /**< 通信错误 */
#define KT_ERR_CRC              ((kt_status_t)-33)      /**< CRC校验错误 */
#define KT_ERR_CHECKSUM         ((kt_status_t)-34)      /**< 校验和错误 */

/* 文件系统错误（预留） */
#define KT_ERR_NOT_FOUND        ((kt_status_t)-40)      /**< 未找到 */
#define KT_ERR_EXISTS           ((kt_status_t)-41)      /**< 已存在 */
#define KT_ERR_PERMISSION       ((kt_status_t)-42)      /**< 权限不足 */
#define KT_ERR_READ             ((kt_status_t)-43)      /**< 读取错误 */
#define KT_ERR_WRITE            ((kt_status_t)-44)      /**< 写入错误 */

/* 队列/缓冲区错误 */
#define KT_ERR_EMPTY            ((kt_status_t)-50)      /**< 空 */
#define KT_ERR_FULL             ((kt_status_t)-51)      /**< 满 */

/** @} */

/*============================================================================
 *                         错误码判断宏
 *============================================================================*/

/**
 * @brief 判断操作是否成功
 */
#define KT_IS_OK(status)        ((status) == KT_OK)

/**
 * @brief 判断操作是否失败
 */
#define KT_IS_ERR(status)       ((status) < KT_OK)

/**
 * @brief 判断错误码是否为特定值
 */
#define KT_IS_ERR_CODE(status, code)  ((status) == (code))

/*============================================================================
 *                         错误处理宏
 *============================================================================*/

/**
 * @brief 如果操作失败则返回错误码
 */
#define KT_RETURN_IF_ERR(expr) \
    do { \
        kt_status_t _ret = (expr); \
        if (KT_IS_ERR(_ret)) { \
            return _ret; \
        } \
    } while(0)

/**
 * @brief 如果条件不满足则返回错误码
 */
#define KT_RETURN_IF_FALSE(cond, err) \
    do { \
        if (!(cond)) { \
            return (err); \
        } \
    } while(0)

/**
 * @brief 如果指针为空则返回错误码
 */
#define KT_RETURN_IF_NULL(ptr, err) \
    do { \
        if ((ptr) == NULL) { \
            return (err); \
        } \
    } while(0)

/*============================================================================
 *                         错误信息（可选）
 *============================================================================*/

#ifdef KT_DEBUG_ENABLE

/**
 * @brief 获取错误码对应的字符串描述
 * @param status 错误码
 * @return 错误描述字符串
 */
const char* kt_error_to_string(kt_status_t status);

#else

#define kt_error_to_string(status)  ""

#endif

#ifdef __cplusplus
}
#endif

#endif /* KT_ERROR_H */