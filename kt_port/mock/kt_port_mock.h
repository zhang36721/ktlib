/**
 * @file kt_port_mock.h
 * @author kento
 * @brief Mock平台头文件
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#ifndef KT_PORT_MOCK_H
#define KT_PORT_MOCK_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Mock平台初始化
 */
void kt_port_mock_init(void);

/**
 * @brief Mock平台反初始化
 */
void kt_port_mock_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* KT_PORT_MOCK_H */