#ifndef TOFSENDOR_OP
#define TOFSENDOR_OP


//�����û�ƽ̨�Զ����ֳ�
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned long U32;
typedef char S8;
typedef short S16;
typedef long S32;

#define	SENSOR_PACKET_START 		0x0a  //���ݱ�ͷ
#define	SENSOR_DATA_LEN 			15		//���������С
typedef struct RO_DATA
{
	U16	distance;
	U16	precision;
	U16	magnitude;
	U8	magnitude_exp;
	U8	ambient_adc;
}sensorData;


//�ⲿ��������
//extern void enableEXINT0(void);//�����ⲿ�жϴ���
//extern void disableEXINT0(void);//�ر��ⲿ�ж�
//extern void uart_write(U8 *writeBuff, U8 len);//����д,���жϷ�ʽ
//extern void uart_read(U8 *readBuff, U8 len, U8 timeoutSec);//���ڶ������жϷ�ʽ

//�ӿ�����
//void ToF_continuousStop(void);//ֹͣ��������
//U8 ToF_SingleshotCapture(sensorData *dbuf);//��������
//U8 ToF_Get_DeviceInfo(U8 *data_buff);//��ȡ��������Ϣ
void ToF_ContinuousStart(void);//��ʼ��������
//U8 ToF_Set_DistanceOffset(S16 offset_mm);//���þ���ƫ��ֵ
//U8 ToF_Set_Filter(S16 val);//���������˲���������
//U8 ToF_Get_AFETemp(U16 *afe_temp);//��ȡAFE�¶�ֵ
U8 ToF_parse(U8 *packet, U16 packet_len, U16 *distance);
#endif
