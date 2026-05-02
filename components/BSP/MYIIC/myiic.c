#include "myiic.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "MYIIC";
i2c_master_bus_handle_t bus_handle;  /* 总线句柄 */
static bool bus_initialized = false; /* 总线初始化状态 */

/**
 * @brief       初始化MYIIC
 * @param       无
 * @retval      ESP_OK:初始化成功
 */
esp_err_t myiic_init(void)
{
    if (bus_initialized)
    {
        ESP_LOGW(TAG, "I2C bus already initialized");
        return ESP_OK;
    }

    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,    /* 时钟源 */
        .i2c_port = IIC_NUM_PORT,             /* I2C端口 */
        .scl_io_num = IIC_SCL_GPIO_PIN,       /* SCL管脚 */
        .sda_io_num = IIC_SDA_GPIO_PIN,       /* SDA管脚 */
        .glitch_ignore_cnt = 7,               /* 故障周期 */
        .flags.enable_internal_pullup = true, /* 内部上拉 */
    };
    /* 新建I2C总线 */
    esp_err_t err = i2c_new_master_bus(&i2c_bus_config, &bus_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to create I2C master bus: %s", esp_err_to_name(err));
        return err;
    }

    bus_initialized = true;
    ESP_LOGI(TAG, "I2C bus initialized successfully (SDA: %d, SCL: %d)", IIC_SDA_GPIO_PIN, IIC_SCL_GPIO_PIN);
    return ESP_OK;
}

/**
 * @brief       反初始化MYIIC
 * @param       无
 * @retval      ESP_OK:反初始化成功
 */
esp_err_t myiic_deinit(void)
{
    if (!bus_initialized)
    {
        return ESP_OK;
    }

    esp_err_t err = i2c_del_master_bus(bus_handle);
    if (err == ESP_OK)
    {
        bus_initialized = false;
        ESP_LOGI(TAG, "I2C bus deinitialized");
    }
    return err;
}

/**
 * @brief       添加I2C设备到总线
 * @param       dev_addr: 设备地址
 * @param       dev_handle: 返回的设备句柄
 * @retval      ESP_OK:添加成功
 */
esp_err_t myiic_add_device(uint16_t dev_addr, i2c_master_dev_handle_t *dev_handle)
{
    if (!bus_initialized)
    {
        ESP_LOGE(TAG, "I2C bus not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = dev_addr,
        .scl_speed_hz = IIC_SPEED_CLK,
    };

    esp_err_t err = i2c_master_bus_add_device(bus_handle, &dev_cfg, dev_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to add device at address 0x%02X: %s", dev_addr, esp_err_to_name(err));
    }
    else
    {
        ESP_LOGI(TAG, "Device added at address 0x%02X", dev_addr);
    }
    return err;
}

/**
 * @brief       从总线移除I2C设备
 * @param       dev_handle: 设备句柄
 * @retval      ESP_OK:移除成功
 */
esp_err_t myiic_remove_device(i2c_master_dev_handle_t dev_handle)
{
    return i2c_master_bus_rm_device(dev_handle);
}

/**
 * @brief       向I2C设备写入数据
 * @param       dev_handle: 设备句柄
 * @param       data: 数据指针
 * @param       len: 数据长度
 * @retval      ESP_OK:写入成功
 */
esp_err_t myiic_write(i2c_master_dev_handle_t dev_handle, const uint8_t *data, size_t len)
{
    return i2c_master_transmit(dev_handle, data, len, pdMS_TO_TICKS(1000));
}

/**
 * @brief       从I2C设备读取数据
 * @param       dev_handle: 设备句柄
 * @param       data: 数据指针
 * @param       len: 数据长度
 * @retval      ESP_OK:读取成功
 */
esp_err_t myiic_read(i2c_master_dev_handle_t dev_handle, uint8_t *data, size_t len)
{
    return i2c_master_receive(dev_handle, data, len, pdMS_TO_TICKS(1000));
}

/**
 * @brief       向I2C设备写入数据后读取数据
 * @param       dev_handle: 设备句柄
 * @param       write_data: 写入数据指针
 * @param       write_len: 写入数据长度
 * @param       read_data: 读取数据指针
 * @param       read_len: 读取数据长度
 * @retval      ESP_OK:操作成功
 */
esp_err_t myiic_write_read(i2c_master_dev_handle_t dev_handle, const uint8_t *write_data, size_t write_len, uint8_t *read_data, size_t read_len)
{
    return i2c_master_transmit_receive(dev_handle, write_data, write_len, read_data, read_len, pdMS_TO_TICKS(1000));
}
