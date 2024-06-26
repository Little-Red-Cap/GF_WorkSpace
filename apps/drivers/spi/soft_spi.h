#ifndef SOFT_SPI_H
#define SOFT_SPI_H
#ifdef __cplusplus
extern "C" {
#endif
#define GF_SOFT_SPI 1
#if GF_SOFT_SPI
typedef struct {
    unsigned char dev_id;
    char *dev_name;
    unsigned char *buff_t;
    unsigned char *buff_r;
} soft_spi_handle_t;

void soft_SPI_Init();
void soft_SPI_Transmit(soft_spi_handle_t *dev, unsigned char *data, unsigned short size);
void soft_SPI_Receive(soft_spi_handle_t *dev, unsigned char *data, unsigned short size);
void soft_SPI_TransmitReceive(soft_spi_handle_t *dev, unsigned char *txData, unsigned char *rxData, unsigned short size);

#endif
#ifdef __cplusplus
}
#endif
#endif //SOFT_SPI_H
