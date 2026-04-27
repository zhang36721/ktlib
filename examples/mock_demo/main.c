/**
 * @file main.c
 * @author kento
 * @brief ktlib Mock 平台测试程序
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include <stdio.h>
#include <string.h>
#include "kt_ringbuf.h"

/** 测试缓冲区大小 */
#define TEST_BUF_SIZE   256

/** 测试数据 */
static const kt_uint8_t test_data[] = {0x01, 0x02, 0x03, 0x04, 0x05, 
                                        0x06, 0x07, 0x08, 0x09, 0x0A};

/** 测试结果统计 */
static int tests_passed = 0;
static int tests_failed = 0;

/**
 * @brief 断言宏（测试用）
 */
#define TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { \
            printf("  [PASS] %s\n", msg); \
            tests_passed++; \
        } else { \
            printf("  [FAIL] %s\n", msg); \
            tests_failed++; \
        } \
    } while(0)

/**
 * @brief 打印分隔线
 */
static void print_separator(void)
{
    printf("----------------------------------------\n");
}

/**
 * @brief 测试1：基本初始化和状态检查
 */
static void test_basic_init(void)
{
    kt_ringbuf_t rb;
    kt_uint8_t buffer[TEST_BUF_SIZE];
    kt_status_t ret;
    
    printf("\n=== Test 1: Basic Init ===\n");
    
    /* 测试正常初始化 */
    ret = kt_ringbuf_init(&rb, buffer, TEST_BUF_SIZE);
    TEST_ASSERT(ret == KT_OK, "Init with valid params");
    TEST_ASSERT(kt_ringbuf_is_empty(&rb) == KT_TRUE, "Buffer is empty after init");
    TEST_ASSERT(kt_ringbuf_is_full(&rb) == KT_FALSE, "Buffer is not full after init");
    TEST_ASSERT(kt_ringbuf_data_len(&rb) == 0, "Data length is 0 after init");
    TEST_ASSERT(kt_ringbuf_free_space(&rb) == TEST_BUF_SIZE, "Free space equals size after init");
    
    /* 测试NULL指针 */
    ret = kt_ringbuf_init(NULL, buffer, TEST_BUF_SIZE);
    TEST_ASSERT(ret == KT_ERR_NULL_PTR, "Init with NULL rb returns error");
    
    ret = kt_ringbuf_init(&rb, NULL, TEST_BUF_SIZE);
    TEST_ASSERT(ret == KT_ERR_NULL_PTR, "Init with NULL buffer returns error");
    
    ret = kt_ringbuf_init(&rb, buffer, 0);
    TEST_ASSERT(ret == KT_ERR_INVALID_PARAM, "Init with size 0 returns error");
}

/**
 * @brief 测试2：单字节读写
 */
static void test_single_byte(void)
{
    kt_ringbuf_t rb;
    kt_uint8_t buffer[TEST_BUF_SIZE];
    kt_status_t ret;
    kt_uint8_t byte;
    
    printf("\n=== Test 2: Single Byte Read/Write ===\n");
    
    kt_ringbuf_init(&rb, buffer, TEST_BUF_SIZE);
    
    /* 写入一个字节 */
    ret = kt_ringbuf_write_byte(&rb, 0x42);
    TEST_ASSERT(ret == KT_OK, "Write single byte");
    TEST_ASSERT(kt_ringbuf_data_len(&rb) == 1, "Data length is 1 after write");
    
    /* 读取一个字节 */
    ret = kt_ringbuf_read_byte(&rb, &byte);
    TEST_ASSERT(ret == KT_OK, "Read single byte");
    TEST_ASSERT(byte == 0x42, "Read value matches written value");
    TEST_ASSERT(kt_ringbuf_is_empty(&rb) == KT_TRUE, "Buffer is empty after read");
    
    /* 从空缓冲区读取 */
    ret = kt_ringbuf_read_byte(&rb, &byte);
    TEST_ASSERT(ret == KT_ERR_EMPTY, "Read from empty buffer returns error");
    
    /* 测试NULL指针 */
    ret = kt_ringbuf_read_byte(NULL, &byte);
    TEST_ASSERT(ret == KT_ERR_NULL_PTR, "Read with NULL rb returns error");
    
    ret = kt_ringbuf_read_byte(&rb, NULL);
    TEST_ASSERT(ret == KT_ERR_NULL_PTR, "Read with NULL byte returns error");
}

/**
 * @brief 测试3：批量读写
 */
static void test_batch_read_write(void)
{
    kt_ringbuf_t rb;
    kt_uint8_t buffer[TEST_BUF_SIZE];
    kt_uint8_t read_data[sizeof(test_data)];
    kt_uint32_t written, read;
    
    printf("\n=== Test 3: Batch Read/Write ===\n");
    
    kt_ringbuf_init(&rb, buffer, TEST_BUF_SIZE);
    
    /* 批量写入 */
    written = kt_ringbuf_write(&rb, test_data, sizeof(test_data));
    TEST_ASSERT(written == sizeof(test_data), "Batch write returns correct count");
    TEST_ASSERT(kt_ringbuf_data_len(&rb) == sizeof(test_data), "Data length matches after batch write");
    
    /* 批量读取 */
    read = kt_ringbuf_read(&rb, read_data, sizeof(read_data));
    TEST_ASSERT(read == sizeof(read_data), "Batch read returns correct count");
    TEST_ASSERT(memcmp(read_data, test_data, sizeof(test_data)) == 0, 
                "Read data matches written data");
    TEST_ASSERT(kt_ringbuf_is_empty(&rb) == KT_TRUE, "Buffer is empty after batch read");
    
    /* 读取0长度 */
    read = kt_ringbuf_read(&rb, read_data, 0);
    TEST_ASSERT(read == 0, "Read 0 length returns 0");
    
    /* 写入0长度 */
    written = kt_ringbuf_write(&rb, test_data, 0);
    TEST_ASSERT(written == 0, "Write 0 length returns 0");
}

/**
 * @brief 测试4：缓冲区满和回绕
 */
static void test_full_and_wrap(void)
{
    kt_ringbuf_t rb;
    kt_uint8_t buffer[10];  /* 小缓冲区用于测试 */
    kt_uint8_t write_data[20];
    kt_uint8_t read_data[10];
    kt_uint32_t written, read;
    int i;
    
    printf("\n=== Test 4: Buffer Full and Wrap Around ===\n");
    
    kt_ringbuf_init(&rb, buffer, sizeof(buffer));
    
    /* 准备测试数据 */
    for (i = 0; i < 20; i++)
    {
        write_data[i] = (kt_uint8_t)i;
    }
    
    /* 写入直到满 */
    written = kt_ringbuf_write(&rb, write_data, 10);
    TEST_ASSERT(written == 10, "Write 10 bytes to 10-byte buffer");
    TEST_ASSERT(kt_ringbuf_is_full(&rb) == KT_TRUE, "Buffer is full");
    
    /* 满缓冲区写入失败 */
    written = kt_ringbuf_write(&rb, write_data, 1);
    TEST_ASSERT(written == 0, "Write to full buffer returns 0");
    
    /* 读取5个字节 */
    read = kt_ringbuf_read(&rb, read_data, 5);
    TEST_ASSERT(read == 5, "Read 5 bytes");
    TEST_ASSERT(kt_ringbuf_is_full(&rb) == KT_FALSE, "Buffer is not full after partial read");
    TEST_ASSERT(kt_ringbuf_free_space(&rb) == 5, "Free space is 5");
    
    /* 再写入5个字节（回绕） */
    written = kt_ringbuf_write(&rb, write_data, 5);
    TEST_ASSERT(written == 5, "Write 5 bytes after partial read");
    TEST_ASSERT(kt_ringbuf_is_full(&rb) == KT_TRUE, "Buffer is full again");
    
    /* 读取所有数据验证回绕 */
    read = kt_ringbuf_read(&rb, read_data, 10);
    TEST_ASSERT(read == 10, "Read 10 bytes after wrap");
    
    /* 验证数据正确性 */
    TEST_ASSERT(read_data[0] == 5 && read_data[1] == 6 && read_data[2] == 7 && 
                read_data[3] == 8 && read_data[4] == 9 &&
                read_data[5] == 0 && read_data[6] == 1 && read_data[7] == 2 && 
                read_data[8] == 3 && read_data[9] == 4,
                "Data order correct after wrap");
}

/**
 * @brief 测试5：Peek操作
 */
static void test_peek(void)
{
    kt_ringbuf_t rb;
    kt_uint8_t buffer[TEST_BUF_SIZE];
    kt_uint8_t peek_data[5];
    kt_uint32_t peeked;
    
    printf("\n=== Test 5: Peek Operation ===\n");
    
    kt_ringbuf_init(&rb, buffer, TEST_BUF_SIZE);
    
    /* 写入测试数据 */
    kt_ringbuf_write(&rb, test_data, sizeof(test_data));
    
    /* Peek数据 */
    peeked = kt_ringbuf_peek(&rb, peek_data, 5);
    TEST_ASSERT(peeked == 5, "Peek 5 bytes");
    TEST_ASSERT(memcmp(peek_data, test_data, 5) == 0, "Peeked data matches");
    TEST_ASSERT(kt_ringbuf_data_len(&rb) == sizeof(test_data), 
                "Data length unchanged after peek");
    
    /* 读取验证 */
    kt_uint8_t read_byte;
    kt_ringbuf_read_byte(&rb, &read_byte);
    TEST_ASSERT(read_byte == test_data[0], "Read after peek returns first byte");
}

/**
 * @brief 测试6：Reset操作
 */
static void test_reset(void)
{
    kt_ringbuf_t rb;
    kt_uint8_t buffer[TEST_BUF_SIZE];
    
    printf("\n=== Test 6: Reset Operation ===\n");
    
    kt_ringbuf_init(&rb, buffer, TEST_BUF_SIZE);
    
    /* 写入一些数据 */
    kt_ringbuf_write(&rb, test_data, sizeof(test_data));
    TEST_ASSERT(kt_ringbuf_is_empty(&rb) == KT_FALSE, "Buffer not empty after write");
    
    /* Reset */
    kt_ringbuf_reset(&rb);
    TEST_ASSERT(kt_ringbuf_is_empty(&rb) == KT_TRUE, "Buffer empty after reset");
    TEST_ASSERT(kt_ringbuf_data_len(&rb) == 0, "Data length 0 after reset");
    
    /* Reset NULL指针（不应崩溃） */
    kt_ringbuf_reset(NULL);
    TEST_ASSERT(1, "Reset NULL does not crash");
}

/**
 * @brief 主函数
 */
int main(void)
{
    printf("============================================\n");
    printf("  ktlib Ring Buffer Test Suite\n");
    printf("  Version: %s\n", KT_VERSION_STRING);
    printf("============================================\n");
    
    /* 运行所有测试 */
    test_basic_init();
    test_single_byte();
    test_batch_read_write();
    test_full_and_wrap();
    test_peek();
    test_reset();
    
    /* 打印结果 */
    print_separator();
    printf("\nTest Results:\n");
    printf("  Passed: %d\n", tests_passed);
    printf("  Failed: %d\n", tests_failed);
    printf("  Total:  %d\n", tests_passed + tests_failed);
    
    if (tests_failed == 0)
    {
        printf("\n*** ALL TESTS PASSED ***\n");
        return 0;
    }
    else
    {
        printf("\n*** SOME TESTS FAILED ***\n");
        return 1;
    }
}