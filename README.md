# ESP32-ADC
通过配置 ESP32 SAR ADC 的 ADC_ATTEN_DB_11 衰减档位，将 0~3.3V 模拟输入映射至 12-bit 数字域。结合 adc_cali 组件加载 eFuse 出厂校准曲线，完成 Raw Code 到电压值的线性转换。在数字域引入滑动平均滤波器，配合前端 RC 低通网络，有效抑制高频量化噪声与工频干扰，提升采样信噪比（SNR）
