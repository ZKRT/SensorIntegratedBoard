/**
  ******************************************************************************
  * @file    ToFSensorOP.c 
  * @author  ZKRT
  * @version V1.0
  * @date    9-May-2017
  * @brief   ����ToF���������ݽ���
	*					 + (1) init
	*                       
  ******************************************************************************
  * @attention
  *
  * ...
  *
  ******************************************************************************
  */

#include "ToFSensorOP.h"
#include "osusart.h"

U16 crc_ta_4[16]={ /* CRC half byte table */
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
};

/**
  * @brief  �������ݰ�CRC16 CCITTУ����
  * @param  
  *			1��ptr�����ݰ��׵�ַ
  *			2��len�����ݰ�Ҫ�������������ֽ���
  * @note   
  * @retval �������ݰ��� CRC16 ֵ
  */
U16 CRC16_Cal(U8* ptr, U8 len)
{
    U16 crc = 0xffff;
	U8 high = 0x00;

    while(len-- != 0)
    {
        high = (U8)(crc/4096);
        crc <<= 4;
        crc ^= crc_ta_4[high^(*ptr/16)];
        high = (U8)(crc/4096);
        crc <<= 4;
        crc ^= crc_ta_4[high^(*ptr&0x0f)];
        ptr++;
    }
    return crc;
}


/**
  * @brief  ���������ص����ݰ�����
  * @param  
  *			1��packet�����ݰ��׵�ַ������Ϊ�̶�Ϊ10���ֽڵ�����
  *			2��data_buff[]���洢����������ݻ�����
  * @note   �ȸ��ݷ���ֵ�ж��Ƿ�����ɹ�����ʹ��data_buff[]������
  *			ע�⣺ȷ��packet��data_buff���ڴ��������ص�
  * @retval �����ɹ����� 1��ʧ�ܷ��� 0
  */
U8 packet_parser(U8 *packet, U8 data_buff[])
{
	U16 crc16_cal = 0x0000;
	U16 crc16_packet = 0x0000;
	U8 len = packet[1]+2;
	if(packet[0] != SENSOR_PACKET_START)
	{
		return 0;
	}
	crc16_packet = packet[len-2]<<8;//ȡ�����ݰ�crc16
	crc16_packet |= packet[len-1];
	crc16_cal = CRC16_Cal(packet+2, len-4);
	if(crc16_cal == crc16_packet)
	{
		memcpy(data_buff, packet+2, len-4);
		return 1;
	}
	return 0;
}


/**
  * @brief  ֹͣ��������
  * @param  ��
  * @note   
  * @retval ��
  */
void ToF_continuousStop(void)
{
//	U8 comm[] = {0x0A, 0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBC, 0x6F};
//	disableEXINT0();//�ر��ⲿ�жϴ������ݽ���
//	uart_write(comm, 10);
}

///**
//  * @brief  ���λ�ȡ�������
//  * @param  dbuf�����ݻ�������ΪsensorData *����
//  * @note   
//  * @retval �ɹ����� 1��ʧ�ܷ��� 0
//  */
//U8 ToF_SingleshotCapture(sensorData *dbuf)
//{
//	U8 comm[SENSOR_DATA_LEN-4] = {0x0A, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAE, 0x57};
//	U8 read_buff[SENSOR_DATA_LEN]={0};
//	SCON0_RI = 0;//���жϷ�ʽ���ڽ��գ������UART �����жϣ� �����Լ�ƽ̨�����Ƿ���Ҫ�����
//	read_buff[0]=0x00;
//	uart_write(comm, 10);
//	uart_read(read_buff, SENSOR_DATA_LEN, 1);
//	memset(dbuf, 0, sizeof(sensorData));
//	//memset(comm, 0, 10);
//	if(packet_parser(read_buff, comm) == 1)
//	{
//		dbuf->distance = (comm[3] << 8)|comm[4];
//		dbuf->magnitude = (comm[5] << 8)|comm[6];
//		dbuf->magnitude_exp = comm[7];
//		dbuf->ambient_adc = comm[8];
//		dbuf->precision = (comm[9] << 8)|comm[10];
//		return 1;
//	}
//	return 0;
//}

///**
//  * @brief  ��ȡ�������豸��Ϣ
//  * @param  data_buff�����ݻ�����
//  * @note   ���ݸ�ʽ�ο�datasheet
//  * @retval �ɹ����� 1��ʧ�ܷ��� 0
//  */
//U8 ToF_Get_DeviceInfo(U8 *data_buff)
//{
//	U8 comm[10]={0x0A, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x3C};
//	U8 read_buff[26] = {0};
//	U8 s_return = 0;
//	SCON0_RI = 0;//���жϷ�ʽ���ڽ��գ������UART �����жϣ� �����Լ�ƽ̨�����Ƿ���Ҫ�����
//	uart_write(comm, 10);
//	uart_read(read_buff, 26, 1);
//	s_return = packet_parser(read_buff, data_buff);
//	return s_return;
//}

/**
  * @brief  ��������ʼ��������
  * @param  ��
  * @note   �Ὺ���ⲿ�жϴ������ݽ���
  * @retval ��
  */
void ToF_ContinuousStart(void)
{
	U8 comm[] = {0x0A, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x72};
//	enableEXINT0();
	t_osscomm_sendMessage(comm, 10, USART1);
	t_osscomm_sendMessage(comm, 10, USART2);
	t_osscomm_sendMessage(comm, 10, USART3);
}

///**
//  * @brief  ���ô���������ƫ��ֵ
//  * @param  offset����λΪ���ף�mm��
//  * @note   ��ͨ���˷���У���������Ľ��������
//  * @retval �ɹ����� 1��ʧ�ܷ��� 0
//  */
//U8 ToF_Set_DistanceOffset(S16 offset_mm)
//{
//					  //��ͷ ����	   key	v_MSB v_LSB  unuse unuse unuse crc_MSB crc_LSB
//	U8 comm[10]={0x0A, 0x38, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
////	U8 read_buff[5] = {0};
//	U8 s_return = 0;
//	U16 crc = 0x0000;
//	comm[3] = offset_mm >> 8;
//	comm[4] = offset_mm;
//	crc = CRC16_Cal(comm, 8);
//	comm[8] = crc >> 8;
//	comm[9] = crc;
//	SCON0_RI = 0;//���жϷ�ʽ���ڽ��գ������UART �����жϣ� �����Լ�ƽ̨�����Ƿ���Ҫ�����
//	uart_write(comm, 10);
//	uart_read(read_buff, 5, 1);
//	packet_parser(read_buff, &s_return);
//	return s_return;
//}


///**
//  * @brief  �����˲���������
//  * @param  val:�˲�����������ֵ
//  * @note   ����ʹ�òο�datasheet
//  * @retval �ɹ����� 1��ʧ�ܷ��� 0
//  */
//U8 ToF_Set_Filter(S16 val)
//{
//					 //��ͷ ����	key	v_MSB v_LSB  unuse unuse unuse crc_MSB crc_LSB
//	U8 comm[10]={0x0A, 0x3D, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
////	U8 read_buff[5] = {0};
//	U8 s_return = 0;
//	U16 crc = 0x0000;
//	comm[3] = val >> 8;
//	comm[4] = val;
//	crc = CRC16_Cal(comm, 8);
//	comm[8] = crc >> 8;
//	comm[9] = crc;
//	uart_write(comm, 10);
//	uart_read(read_buff, 5, 1);
//	packet_parser(read_buff, &s_return);
//	return s_return;
//}

///**
//  * @brief  ��ȡAFE�¶�ֵ
//  * @param  afe_temp���¶�ֵ�洢������,�޷���������ʵ���¶�ֵ���ڴ�ֵ����100
//  * @note   �ȸ��ݷ���ֵ�ж��Ƿ�����ɹ�����ʹ��afe_temp������������
//  * @retval �ɹ����� 1��ʧ�ܷ��� 0
//  */
//U8 ToF_Get_AFETemp(U16 *afe_temp)
//{
//	U8 comm[10]={0x0A, 0x3F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x86};
//	U8 read_buff[6] = {0};
//	U8 data_buff[2] = {0};
//	SCON0_RI = 0;
//	uart_write(comm, 10);
//	uart_read(read_buff, 6, 1);//���ذ�Ϊ6���ֽ�
//	if(1 == packet_parser(read_buff, data_buff))
//	{
//		*afe_temp = (data_buff[0] << 8)|data_buff[1];
//		return 1;
//	}
//	return 0;
//}

//zkrt fuction start
/**
  * @brief  ���������ص����ݰ�����
  * @param  
  *			1��packet�����ݰ��׵�ַ
  * @note   
  * @retval �����ɹ����� 1��ʧ�ܷ��� 0
  */
U8 ToF_parse(U8 *packet, U16 packet_len, U16 *distance)
{
	U16 crc16_cal = 0x0000;
	U16 crc16_packet = 0x0000;
	U8 len = packet[1]+2;
	if(packet[0] != SENSOR_PACKET_START)
	{
		return 0;
	}
	if(len!= 0x0D)  //�������ݳ���Ϊ0x0D
	{
		return 0;
	}	
	crc16_packet = packet[len-2]<<8;//ȡ�����ݰ�crc16
	crc16_packet |= packet[len-1];
	crc16_cal = CRC16_Cal(packet+2, len-4);
	if(crc16_cal != crc16_packet)
	{
		return 0;
	}
	sensorData dbuf;
	dbuf.distance = (packet[5] << 8)|packet[6];
	dbuf.magnitude = (packet[7] << 8)|packet[8];
	dbuf.magnitude_exp = packet[9];
	dbuf.ambient_adc = packet[10];
	dbuf.precision = (packet[11] << 8)|packet[12];

	if(dbuf.distance >=32000)  //������
	{
		return 0;
	}	
	
	if((float)((dbuf.magnitude)/100) < 43) //������
	{
		return 0;
	}
	*distance = dbuf.distance;
	return 1;	
}
//zkrt fuction end
