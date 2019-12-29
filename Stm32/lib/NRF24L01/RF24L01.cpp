/*
    GT-24 V1.0 (RF24L01) ���Կ�������

    ���ߣ����ƿƼ����޹�˾
    �Ա�����:https://shop532831232.taobao.com
	
    ����оƬ�� STM32f103ϵ�� 
	
   ��·���ӣ�  RF24L01�� VCC        3.3V
			            GND        GND
			            CE         A0
						IRQ        A1
                        CSN        B12
						SCK        B13
						MOSI       B15
						MISO       B14
						
	  		 OLED(IIC): VCC        3.3V
					 	GND        GND
					 	SCL        A5
					 	SDA        A7
						
    ����޸�ʱ�䣻2018��8��1�� 12:00:00
  ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*/
  
  
#include "RF24L01.h"


const char *g_ErrorString = "RF24L01 is not find !...";


/**
  * @brief :NRF24L01���Ĵ���
  * @param :
           @Addr:�Ĵ�����ַ
  * @note  :��ַ���豸����Ч
  * @retval:��ȡ������
  */
uint8_t NRF24L01::Read_Reg( uint8_t RegAddr )
{
    int16_t btmp;
	
    RF24L01_CS_LOW();			//Ƭѡ
	
    Spi1SendAndRecvByte( NRF_READ_REG | RegAddr );	//������ ��ַ
    btmp = Spi1SendAndRecvByte( 0xFF );				//������
	
    RF24L01_CS_HIGH( );			//ȡ��Ƭѡ
	
    return btmp;
}

/**
  * @brief :NRF24L01��ָ�����ȵ�����
  * @param :
  *			@reg:��ַ
  *			@pBuf:���ݴ�ŵ�ַ
  *			@len:���ݳ���
  * @note  :���ݳ��Ȳ�����255����ַ���豸����Ч
  * @retval:��ȡ״̬
  */
void NRF24L01::Read_Buf( uint8_t RegAddr, uint8_t *pBuf, uint8_t len )
{
    uint8_t btmp;
	
    RF24L01_CS_LOW( );			//Ƭѡ
	
    Spi1SendAndRecvByte( NRF_READ_REG | RegAddr );	//������ ��ַ
    for( btmp = 0; btmp < len; btmp ++ )
    {
        *( pBuf + btmp ) = Spi1SendAndRecvByte( 0xFF );	//������
    }
    RF24L01_CS_HIGH( );		//ȡ��Ƭѡ
}

/**
  * @brief :NRF24L01д�Ĵ���
  * @param :��
  * @note  :��ַ���豸����Ч
  * @retval:��д״̬
  */
void NRF24L01::Write_Reg( uint8_t RegAddr, uint8_t Value )
{
    RF24L01_CS_LOW( );		//Ƭѡ
	
    Spi1SendAndRecvByte( NRF_WRITE_REG | RegAddr );	//д���� ��ַ
    Spi1SendAndRecvByte( Value );			//д����
	
    RF24L01_CS_HIGH( );		//ȡ��Ƭѡ
}

/**
  * @brief :NRF24L01дָ�����ȵ�����
  * @param :
  *			@reg:��ַ
  *			@pBuf:д������ݵ�ַ
  *			@len:���ݳ���
  * @note  :���ݳ��Ȳ�����255����ַ���豸����Ч
  * @retval:д״̬
  */
void NRF24L01::Write_Buf( uint8_t RegAddr, uint8_t *pBuf, uint8_t len )
{
    uint8_t i;
	
    RF24L01_CS_LOW( );		//Ƭѡ
	
    Spi1SendAndRecvByte( NRF_WRITE_REG | RegAddr );	//д���� ��ַ
    for( i = 0; i < len; i ++ )
    {
        Spi1SendAndRecvByte( *( pBuf + i ) );		//д����
    }
	
    RF24L01_CS_HIGH( );		//ȡ��Ƭѡ
}

/**
  * @brief :���TX������
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01::Flush_Tx_Fifo ( void )
{
    RF24L01_CS_LOW( );		//Ƭѡ
	
    Spi1SendAndRecvByte( FLUSH_TX );	//��TX FIFO����
	
    RF24L01_CS_HIGH( );		//ȡ��Ƭѡ
}

/**
  * @brief :���RX������
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01::Flush_Rx_Fifo( void )
{
    RF24L01_CS_LOW( );		//Ƭѡ
	
    Spi1SendAndRecvByte( FLUSH_RX );	//��RX FIFO����
	
    RF24L01_CS_HIGH( );		//ȡ��Ƭѡ
}

/**
  * @brief :����ʹ����һ������
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01::Reuse_Tx_Payload( void )
{
    RF24L01_CS_LOW( );		//Ƭѡ
	
    Spi1SendAndRecvByte( REUSE_TX_PL );		//����ʹ����һ������
	
    RF24L01_CS_HIGH( );		//ȡ��Ƭѡ
}

/**
  * @brief :NRF24L01�ղ���
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01::Nop( void )
{
    RF24L01_CS_LOW( );		//Ƭѡ
	
    Spi1SendAndRecvByte( NOP );		//�ղ�������
	
    RF24L01_CS_HIGH( );		//ȡ��Ƭѡ
}

/**
  * @brief :NRF24L01��״̬�Ĵ���
  * @param :��
  * @note  :��
  * @retval:RF24L01״̬
  */
uint8_t NRF24L01::Read_Status_Register( void )
{
    uint8_t Status;
	
    RF24L01_CS_LOW( );		//Ƭѡ
	
    Status = Spi1SendAndRecvByte( NRF_READ_REG + STATUS );	//��״̬�Ĵ���
	
    RF24L01_CS_HIGH( );		//ȡ��Ƭѡ
	
    return Status;
}

/**
  * @brief :NRF24L01���ж�
  * @param :
           @IRQ_Source:�ж�Դ
  * @note  :��
  * @retval:�����״̬�Ĵ�����ֵ
  */
uint8_t NRF24L01::Clear_IRQ_Flag( uint8_t IRQ_Source )
{
    uint8_t btmp = 0;

    IRQ_Source &= ( 1 << RX_DR ) | ( 1 << TX_DS ) | ( 1 << MAX_RT );	//�жϱ�־����
    btmp = Read_Status_Register( );			//��״̬�Ĵ���
			
    RF24L01_CS_LOW( );			//Ƭѡ
    Spi1SendAndRecvByte( NRF_WRITE_REG + STATUS );	//д״̬�Ĵ�������
    Spi1SendAndRecvByte( IRQ_Source | btmp );		//����Ӧ�жϱ�־
    RF24L01_CS_HIGH( );			//ȡ��Ƭѡ
	
    return ( Read_Status_Register( ));			//����״̬�Ĵ���״̬
}

/**
  * @brief :��RF24L01�ж�״̬
  * @param :��
  * @note  :��
  * @retval:�ж�״̬
  */
uint8_t NRF24L01::Read_IRQ_Status( void )
{
    return ( Read_Status_Register( ) & (( 1 << RX_DR ) | ( 1 << TX_DS ) | ( 1 << MAX_RT )));	//�����ж�״̬
}
 
 /**
  * @brief :��FIFO�����ݿ��
  * @param :��
  * @note  :��
  * @retval:���ݿ��
  */
uint8_t NRF24L01::Read_Top_Fifo_Width( void )
{
    uint8_t btmp;
	
    RF24L01_CS_LOW( );		//Ƭѡ
	
    Spi1SendAndRecvByte( R_RX_PL_WID );	//��FIFO�����ݿ������
    btmp = Spi1SendAndRecvByte( 0xFF );	//������
	
    RF24L01_CS_HIGH( );		//ȡ��Ƭѡ
	
    return btmp;
}

 /**
  * @brief :�����յ�������
  * @param :��
  * @note  :��
  * @retval:
           @pRxBuf:���ݴ�ŵ�ַ�׵�ַ
  */
uint8_t NRF24L01::Read_Rx_Payload( uint8_t *pRxBuf )
{
    uint8_t Width, PipeNum;
	
    PipeNum = ( Read_Reg( STATUS ) >> 1 ) & 0x07;	//������״̬
    Width = Read_Top_Fifo_Width( );		//���������ݸ���

    RF24L01_CS_LOW( );		//Ƭѡ
    Spi1SendAndRecvByte( RD_RX_PLOAD );			//����Ч��������
	
    for( PipeNum = 0; PipeNum < Width; PipeNum ++ )
    {
        *( pRxBuf + PipeNum ) = Spi1SendAndRecvByte( 0xFF );		//������
    }
    RF24L01_CS_HIGH( );		//ȡ��Ƭѡ
    Flush_Rx_Fifo( );	//���RX FIFO
	
    return Width;
}

 /**
  * @brief :�������ݣ���Ӧ��
  * @param :
  *			@pTxBuf:�������ݵ�ַ
  *			@len:����
  * @note  :һ�β�����32���ֽ�
  * @retval:��
  */
void NRF24L01::Write_Tx_Payload_Ack( uint8_t *pTxBuf, uint8_t len )
{
    uint8_t btmp;
    uint8_t length = ( len > 32 ) ? 32 : len;		//���ݳ����Լ32 ��ֻ����32��

    Flush_Tx_Fifo( );		//��TX FIFO
	
    RF24L01_CS_LOW( );			//Ƭѡ
    Spi1SendAndRecvByte( WR_TX_PLOAD );	//��������
	
    for( btmp = 0; btmp < length; btmp ++ )
    {
        Spi1SendAndRecvByte( *( pTxBuf + btmp ) );	//��������
    }
    RF24L01_CS_HIGH( );			//ȡ��Ƭѡ
}

 /**
  * @brief :�������ݣ�����Ӧ��
  * @param :
  *			@pTxBuf:�������ݵ�ַ
  *			@len:����
  * @note  :һ�β�����32���ֽ�
  * @retval:��
  */
void NRF24L01::Write_Tx_Payload_NoAck( uint8_t *pTxBuf, uint8_t len )
{
    if( len > 32 || len == 0 )
    {
        return ;		//���ݳ��ȴ���32 ���ߵ���0 ��ִ��
    }
	
    RF24L01_CS_LOW( );	//Ƭѡ
    Spi1SendAndRecvByte( WR_TX_PLOAD_NACK );	//��������
    while( len-- )
    {
        Spi1SendAndRecvByte( *pTxBuf );			//��������
		pTxBuf++;
    }
    RF24L01_CS_HIGH( );		//ȡ��Ƭѡ
}

 /**
  * @brief :�ڽ���ģʽ����TX FIFOд����(��ACK)
  * @param :
  *			@pData:���ݵ�ַ
  *			@len:����
  * @note  :һ�β�����32���ֽ�
  * @retval:��
  */
void NRF24L01::Write_Tx_Payload_InAck( uint8_t *pData, uint8_t len )
{
    uint8_t btmp;
	
	len = ( len > 32 ) ? 32 : len;		//���ݳ��ȴ���32����ֻд32���ֽ�

    RF24L01_CS_LOW( );			//Ƭѡ
    Spi1SendAndRecvByte( W_ACK_PLOAD );		//����
    for( btmp = 0; btmp < len; btmp ++ )
    {
        Spi1SendAndRecvByte( *( pData + btmp ) );	//д����
    }
    RF24L01_CS_HIGH( );			//ȡ��Ƭѡ
}

 /**
  * @brief :���÷��͵�ַ
  * @param :
  *			@pAddr:��ַ��ŵ�ַ
  *			@len:����
  * @note  :��
  * @retval:��
  */
void NRF24L01::Set_TxAddr( uint8_t *pAddr, uint8_t len )
{
	len = ( len > 5 ) ? 5 : len;					//��ַ���ܴ���5���ֽ�
    	Write_Buf( TX_ADDR, pAddr, len );	//д��ַ
}

 /**
  * @brief :���ý���ͨ����ַ
  * @param :
  *			@PipeNum:ͨ��
  *			@pAddr:��ַ����ŵ�ַ
  *			@Len:����
  * @note  :ͨ��������5 ��ַ���Ȳ�����5���ֽ�
  * @retval:��
  */
void NRF24L01::Set_RxAddr( uint8_t PipeNum, uint8_t *pAddr, uint8_t Len )
{
    Len = ( Len > 5 ) ? 5 : Len;
    PipeNum = ( PipeNum > 5 ) ? 5 : PipeNum;		//ͨ��������5 ��ַ���Ȳ�����5���ֽ�

    Write_Buf( RX_ADDR_P0 + PipeNum, pAddr, Len );	//д���ַ
}

 /**
  * @brief :����ͨ���ٶ�
  * @param :
  *			@Speed:�ٶ�
  * @note  :��
  * @retval:��
  */
void NRF24L01::Set_Speed( nRf24l01SpeedType Speed )
{
	uint8_t btmp = 0;
	
	btmp = Read_Reg( RF_SETUP );
	btmp &= ~( ( 1<<5 ) | ( 1<<3 ) );
	
	if( Speed == SPEED_250K )		//250K
	{
		btmp |= ( 1<<5 );
	}
	else if( Speed == SPEED_1M )   //1M
	{
   		btmp &= ~( ( 1<<5 ) | ( 1<<3 ) );
	}
	else if( Speed == SPEED_2M )   //2M
	{
		btmp |= ( 1<<3 );
	}

	NRF24L01::Write_Reg( RF_SETUP, btmp );
}

 /**
  * @brief :���ù���
  * @param :
  *			@Speed:�ٶ�
  * @note  :��
  * @retval:��
  */
void NRF24L01::Set_Power( nRf24l01PowerType Power )
{
	uint8_t btmp;

	btmp = Read_Reg( RF_SETUP ) & ~0x07;
	switch( Power )
	{
	        case POWER_F18DBM:
		            btmp |= PWR_18DB;
		            break;
	        case POWER_F12DBM:
		            btmp |= PWR_12DB;
		            break;
	        case POWER_F6DBM:
		            btmp |= PWR_6DB;
		            break;
	        case POWER_0DBM:
	            	btmp |= PWR_0DB;
	            	break;
	        default:
	            	break;
	}
	Write_Reg( RF_SETUP, btmp );
}

 /**
  * @brief :����Ƶ��
  * @param :
  *			@FreqPoint:Ƶ�����ò���
  * @note  :ֵ������127
  * @retval:��
  */
void NRF24L01::Write_Hopping_Point( uint8_t FreqPoint )
{
    Write_Reg(  RF_CH, FreqPoint & 0x7F );
}

/**
  * @brief :NRF24L01���
  * @param :��
  * @note  :��
  * @retval:��
  */ 
void NRF24L01::check( void )
{
	uint8_t i;
	uint8_t buf[5]={ 0XA5, 0XA5, 0XA5, 0XA5, 0XA5 };
	uint8_t read_buf[ 5 ] = { 0 };
	 
	while( 1 )
	{
		Write_Buf( TX_ADDR, buf, 5 );			//д��5���ֽڵĵ�ַ
		Read_Buf( TX_ADDR, read_buf, 5 );		//����д��ĵ�ַ  
		for( i = 0; i < 5; i++ )
		{
			if( buf[ i ] != read_buf[ i ] )
			{
				break;
			}	
		} 
		
		if( 5 == i )
		{
			break;
		}
		else
		{
			//drv_uart_tx_bytes( (uint8_t *)g_ErrorString, 26 );
		}
		delay_ms(1500);
	}
}

 /**
  * @brief :����ģʽ
  * @param :
  *			@Mode:ģʽ����ģʽ�����ģʽ
  * @note  :��
  * @retval:��
  */
void NRF24L01::Set_Mode( nRf24l01ModeType Mode )
{
    	uint8_t controlreg = 0;
	controlreg = Read_Reg( CONFIG );
	
    	if( Mode == MODE_TX )       
	{
		controlreg &= ~( 1<< PRIM_RX );
	}
    	else 
	{
		if( Mode == MODE_RX )  
		{ 
			controlreg |= ( 1<< PRIM_RX ); 
		}
	}

   	Write_Reg( CONFIG, controlreg );
}

/**
  * @brief :NRF24L01����һ������
  * @param :
  *			@txbuf:�����������׵�ַ
  *			@Length:�������ݳ���
  * @note  :��
  * @retval:
  *			MAX_TX���ﵽ����ط�����
  *			TX_OK���������
  *			0xFF:����ԭ��
  */ 
uint8_t NRF24L01::TxPacket( uint8_t *txbuf, uint8_t Length )
{
	uint8_t l_Status = 0;
	uint16_t l_MsTimes = 0;
	
	RF24L01_CS_LOW( );		//Ƭѡ
	Spi1SendAndRecvByte( FLUSH_TX );
	RF24L01_CS_HIGH( );
	
	RF24L01_CE_LOW( );		
	Write_Buf( WR_TX_PLOAD, txbuf, Length );	//д���ݵ�TX BUF 32�ֽ�  TX_PLOAD_WIDTH
	RF24L01_CE_HIGH( );			//��������
	while( 0 != RF24L01_IRQ_GET_STATUS( ))
	{
		delay_ms( 1 );
		if( 500 == l_MsTimes++ )						//500ms��û�з��ͳɹ������³�ʼ���豸
		{
			Gpio_Init( );
			Init( );
			Set_Mode( MODE_TX );
			break;
		}
	}
	l_Status = Read_Reg(STATUS);						//��״̬�Ĵ���
	Write_Reg( STATUS, l_Status );						//���TX_DS��MAX_RT�жϱ�־
	
	if( l_Status & MAX_TX )	//�ﵽ����ط�����
	{
		Write_Reg( FLUSH_TX,0xff );	//���TX FIFO�Ĵ���
		return MAX_TX; 
	}
	if( l_Status & TX_OK )	//�������
	{
		return TX_OK;
	}
	
	return 0xFF;	//����ԭ����ʧ��
}

/**
  * @brief :NRF24L01��������
  * @param :
  *			@rxbuf:�������ݴ�ŵ�ַ
  * @note  :��
  * @retval:���յ����ݸ���
  */ 
uint8_t NRF24L01::RxPacket( uint8_t *rxbuf )
{
	uint8_t l_Status = 0, l_RxLength = 0, l_100MsTimes = 0;
	
	RF24L01_CS_LOW( );		//Ƭѡ
	Spi1SendAndRecvByte( FLUSH_RX );
	RF24L01_CS_HIGH( );
	
	while( 0 != RF24L01_IRQ_GET_STATUS( ))
	{
		delay_ms( 100 );
		
		if( 30 == l_100MsTimes++ )		//3sû���չ����ݣ����³�ʼ��ģ��
		{
			Gpio_Init( );
			Init( );
			Set_Mode( MODE_RX );
			break;
		}
	}
	
	l_Status = Read_Reg( STATUS );		//��״̬�Ĵ���
	Write_Reg( STATUS,l_Status );		//���жϱ�־
	if( l_Status & RX_OK)	//���յ�����
	{
		l_RxLength = Read_Reg( R_RX_PL_WID );		//��ȡ���յ������ݸ���
		Read_Buf( RD_RX_PLOAD,rxbuf,l_RxLength );	//���յ����� 
		Write_Reg( FLUSH_RX,0xff );				//���RX FIFO
		return l_RxLength; 
	}	
	
	return 0;				//û���յ�����	
}

 /**
  * @brief :RF24L01���ų�ʼ��
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01::Gpio_Init( void )
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = RF24L01_CE_GPIO_PIN ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = RF24L01_CS_GPIO_PIN;
	HAL_GPIO_Init(RF24L01_CS_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = RF24L01_IRQ_GPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	RF24L01_CE_LOW( );		//??24L01
	RF24L01_CS_HIGH( );		//??SPI??
}

 /**
  * @brief :RF24L01ģ���ʼ��
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01::Init( void )
{
	Gpio_Init(  );
	
	check();
		
	uint8_t addr[5] = {INIT_ADDR};

	RF24L01_CE_HIGH( );
	Clear_IRQ_Flag( IRQ_ALL );

	Write_Reg( DYNPD, ( 1 << 0 ) ); 	//ʹ��ͨ��1��̬���ݳ���
	Write_Reg( FEATRUE, 0x07 );
	Read_Reg( DYNPD );
	Read_Reg( FEATRUE );

	Write_Reg( CONFIG, /*( 1<<MASK_RX_DR ) |*/		//�����ж�
	                          ( 1 << EN_CRC ) |     //ʹ��CRC 1���ֽ�
	                          ( 1 << PWR_UP ) );    //�����豸
	Write_Reg( EN_AA, ( 1 << ENAA_P0 ) );   		//ͨ��0�Զ�Ӧ��
	Write_Reg( EN_RXADDR, ( 1 << ERX_P0 ) );		//ͨ��0����
	Write_Reg( SETUP_AW, AW_5BYTES );     			//��ַ��� 5���ֽ�
	Write_Reg( SETUP_RETR, ARD_4000US |
	            ( REPEAT_CNT & 0x0F ) );         	//�ظ��ȴ�ʱ�� 250us
	Write_Reg( RF_CH, 60 );             			//��ʼ��ͨ��
	Write_Reg( RF_SETUP, 0x26 );

	Set_TxAddr( &addr[0], 5 );                      //����TX��ַ
	Set_RxAddr( 0, &addr[0], 5 );                   //����RX��ַ


}
